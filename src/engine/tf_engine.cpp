// Copyright 2023 zh.luxu1986@gmail.com

#include "infer_engine/src/engine/tf_engine.h"

#include <fstream>
#include <string>

#include "glog/logging.h"
#include "tensorflow/c/c_api.h"
#include "tensorflow/core/protobuf/config.pb.h"

#include "infer_engine/src/util/functional/scope_exit_task.h"

namespace infer_engine {

TFEngine::TFEngine(const ModelSpec& model_spec) :
  Engine(model_spec),
  graph_buffer_(nullptr),
  graph_(nullptr),
  session_opts_(nullptr),
  session_(nullptr) {
  init();
}

TFEngine::~TFEngine() {
  try {
    if (nullptr != session_) {
      TF_Status *tf_status = TF_NewStatus();
      ScopeExitTask delete_tf_status([&tf_status]() { TF_DeleteStatus(tf_status); });

      TF_CloseSession(session_, tf_status);
      if (TF_GetCode(tf_status) != TF_OK) {
        const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
          + model_spec_.brief() + "] " + "Failed to close session: " + std::string(TF_Message(tf_status));
        throw std::runtime_error(err_msg);
      }
      LOG(INFO) << "[" << model_spec_.brief() << "] Session closed";

      TF_DeleteSession(session_, tf_status);
      if (TF_GetCode(tf_status) != TF_OK) {
        const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
          + model_spec_.brief() + "] " + "Failed to delete session: " + std::string(TF_Message(tf_status));
        throw std::runtime_error(err_msg);
      }
      session_ = nullptr;
      LOG(INFO) << "[" << model_spec_.brief() << "] Session deleted";
    }

    if (nullptr != session_opts_) {
      TF_DeleteSessionOptions(session_opts_);
      session_opts_ = nullptr;
      LOG(INFO) << "[" << model_spec_.brief() << "] Session options deleted";
    }

    if (nullptr != graph_) {
      TF_DeleteGraph(graph_);
      graph_ = nullptr;
      LOG(INFO) << "[" << model_spec_.brief() << "] Graph deleted";
    }

    if (nullptr != graph_buffer_) {
      TF_DeleteBuffer(graph_buffer_);
      graph_buffer_ = nullptr;
      LOG(INFO) << "[" << model_spec_.brief() << "] Graph buffer deleted";
    }
  } catch (const std::exception& e) {
    LOG(ERROR) << e.what();
  } catch (...) {
    LOG(ERROR) << "Unknown exception";
  }
}

std::string TFEngine::brand() {
  return kBrandTF;
}

void TFEngine::infer(const int32_t batch_size, const void *input, void *output) {
  run_session();
}

void TFEngine::infer(const BatchInstance& batch_instance, BatchScore *batch_score) {
  // Convert BatchInstance to TF_Output and TF_Tensor
}

void TFEngine::trace() {
  tensorflow::RunOptions tf_run_opts;
  tf_run_opts.set_trace_level(tensorflow::RunOptions_TraceLevel_FULL_TRACE);
  std::string tf_run_opts_str;
  tf_run_opts.SerializeToString(&tf_run_opts_str);
  TF_Buffer *tf_run_opts_data = TF_NewBufferFromString(
    reinterpret_cast<void*>(tf_run_opts_str.data()), tf_run_opts_str.size()
  );  // NOLINT
  ScopeExitTask delete_tf_run_opts_data([&tf_run_opts_data]() { TF_DeleteBuffer(tf_run_opts_data); });

  TF_Buffer *tf_metadata = TF_NewBuffer();
  ScopeExitTask delete_tf_metadata([&tf_metadata]() { TF_DeleteBuffer(tf_metadata); });

  run_session(tf_run_opts_data, tf_metadata);

  static std::mutex trace_data_mtx;
  static std::atomic<int> trace_data_index = 0;
  std::lock_guard<std::mutex> lock(trace_data_mtx);
  trace_data_index.fetch_add(1);
  std::string trace_data_str(reinterpret_cast<const char*>(tf_metadata->data), tf_metadata->length);
  std::ofstream ofs("trace_data_" + std::to_string(trace_data_index.load()) + ".pb");
  ofs << trace_data_str;
  ofs.close();
}

void TFEngine::run_session(TF_Buffer *tf_run_opts, TF_Buffer *tf_metadata) {
  TF_Output *inputs = nullptr, *outputs = nullptr;
  TF_Tensor **input_tensors = nullptr, **output_tensors = nullptr;
  int n_inputs = 0, n_outputs = 0;

  TF_Status *tf_status = TF_NewStatus();
  ScopeExitTask delete_tf_status([&tf_status]() { TF_DeleteStatus(tf_status); });

  TF_SessionRun(
    session_, tf_run_opts,
    inputs, input_tensors, n_inputs,
    outputs, output_tensors, n_outputs,
    nullptr, 0, tf_metadata, tf_status
  );  // NOLINT

  if (TF_GetCode(tf_status) != TF_OK) {
    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
      + model_spec_.brief() + "] " + "Failed to run session: " + std::string(TF_Message(tf_status));
    throw std::runtime_error(err_msg);
  }
}

void TFEngine::load() {
  const std::string& graph_file = model_spec_.graph_file;
  std::ifstream file(graph_file, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
      + model_spec_.brief() + "] " + "Failed to open graph file: " + graph_file;
    throw std::runtime_error(err_msg);
  }
  ScopeExitTask close_file([&file]() { file.close(); });

  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  char *buffer_data = new char[size];
  if (!file.read(buffer_data, size)) {
    delete[] buffer_data;

    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
      + model_spec_.brief() + "] " + "Failed to read graph file: " + graph_file;
    throw std::runtime_error(err_msg);
  }

  graph_buffer_ = new TF_Buffer();
  graph_buffer_->data = buffer_data;
  graph_buffer_->length = size;
  graph_buffer_->data_deallocator = [](void *data, size_t length) {
    delete[] static_cast<char*>(data);
  };

  LOG(INFO) << "[" << model_spec_.brief() << "] Graph file loaded: " << graph_file;
}

void TFEngine::build() {
  TF_Status *tf_status = TF_NewStatus();
  TF_ImportGraphDefOptions *tf_import_graph_def_opts = TF_NewImportGraphDefOptions();
  ScopeExitTask delete_tf_status([&tf_status, &tf_import_graph_def_opts]() {
     TF_DeleteStatus(tf_status);
     TF_DeleteImportGraphDefOptions(tf_import_graph_def_opts);
  });

  graph_ = TF_NewGraph();
  TF_GraphImportGraphDef(graph_, graph_buffer_, tf_import_graph_def_opts, tf_status);
  if (TF_GetCode(tf_status) != TF_OK) {
    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
      + model_spec_.brief() + "] " + "Failed to import graph: " + std::string(TF_Message(tf_status));
    throw std::runtime_error(err_msg);
  }

  LOG(INFO) << "[" << model_spec_.brief() << "] Graph imported";
}

void TFEngine::set_session_options() {
  TF_Status *tf_status = TF_NewStatus();
  ScopeExitTask delete_tf_status([&tf_status] {
     TF_DeleteStatus(tf_status);
  });

  tensorflow::OptimizerOptions tf_optimizer_opts;
  tf_optimizer_opts.set_do_constant_folding(true);
  tf_optimizer_opts.set_do_function_inlining(true);
  tf_optimizer_opts.set_opt_level(tensorflow::OptimizerOptions_Level_L1);
  tf_optimizer_opts.set_global_jit_level(tensorflow::OptimizerOptions_GlobalJitLevel_ON_2);

  tensorflow::ConfigProto tf_session_conf;
  tf_session_conf.mutable_graph_options()->mutable_optimizer_options()->CopyFrom(tf_optimizer_opts);
  tf_session_conf.set_intra_op_parallelism_threads(1);
  tf_session_conf.set_inter_op_parallelism_threads(1);

  std::string tf_session_conf_str;
  tf_session_conf.SerializeToString(&tf_session_conf_str);

  session_opts_ = TF_NewSessionOptions();
  TF_SetConfig(session_opts_, tf_session_conf_str.data(), tf_session_conf_str.size(), tf_status);
  if (TF_GetCode(tf_status) != TF_OK) {
    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
      + model_spec_.brief() + "] " + "Failed to set session config: " + std::string(TF_Message(tf_status));
    throw std::runtime_error(err_msg);
  }

  LOG(INFO) << "[" << model_spec_.brief() << "] Session options set";
}

void TFEngine::create_session() {
  TF_Status *tf_status = TF_NewStatus();
  ScopeExitTask delete_tf_status([&tf_status] {
     TF_DeleteStatus(tf_status);
  });

  session_ = TF_NewSession(graph_, session_opts_, tf_status);
  if (TF_GetCode(tf_status) != TF_OK) {
    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
      + model_spec_.brief() + "] " + "Failed to create session: " + std::string(TF_Message(tf_status));
    throw std::runtime_error(err_msg);
  }

  LOG(INFO) << "[" << model_spec_.brief() << "] Session created";
}

void TFEngine::sub_init() {
  TF_Status *tf_status = TF_NewStatus();
  ScopeExitTask delete_tf_status([&tf_status] {
     TF_DeleteStatus(tf_status);
  });
}

// void TFEngine::print_graph_info() {
//   int n_ops = TF_GraphNumOperations(graph_);
//   for (int i = 0; i < n_ops; ++i) {
//     TF_Operation *op = TF_GraphGetOperationByIndex(graph_, i);
//     const char *op_name = TF_OperationName(op);
//     const char *op_type = TF_OperationOpType(op);
//     int n_inputs = TF_OperationNumInputs(op);
//     int n_outputs = TF_OperationNumOutputs(op);
//     LOG(INFO) << "[" << model_spec_.brief() << "] Operation: " << op_name << " (" << op_type << ")";
//     LOG(INFO) << "[" << model_spec_.brief() << "]   Inputs: " << n_inputs;
//     LOG(INFO) << "[" << model_spec_.brief() << "]   Outputs: " << n_outputs;
//   }
//
//   int n_inputs = TF_GraphNumInputs(graph_);
//   int n_outputs = TF_GraphNumOutputs(graph_);
//   LOG(INFO) << "[" << model_spec_.brief() << "] Inputs: " << n_inputs;
//   LOG(INFO) << "[" << model_spec_.brief() << "] Outputs: " << n_outputs;
// }
//
// void TFEngine::get_input_output_ops() {
//   TF_Status *tf_status = TF_NewStatus();
//   ScopeExitTask delete_tf_status([&tf_status] {
//      TF_DeleteStatus(tf_status);
//   });
//
//   int n_inputs = TF_GraphNumInputs(graph_);
//   int n_outputs = TF_GraphNumOutputs(graph_);
//
//   TF_Output *inputs = new TF_Output[n_inputs];
//   ScopeExitTask delete_inputs([&inputs] { delete[] inputs; });
//
//   TF_Output *outputs = new TF_Output[n_outputs];
//   ScopeExitTask delete_outputs([&outputs] { delete[] outputs; });
//
//   for (int i = 0; i < n_inputs; ++i) {
//     inputs[i] = TF_GraphGetInput(graph_, i);
//   }
//   for (int i = 0; i < n_outputs; ++i) {
//     outputs[i] = TF_GraphGetOutput(graph_, i);
//   }
//
//   TF_Operation **input_ops = new TF_Operation*[n_inputs];
//   ScopeExitTask delete_input_ops([&input_ops] { delete[] input_ops; });
//
//   TF_Operation **output_ops = new TF_Operation*[n_outputs];
//   ScopeExitTask delete_output_ops([&output_ops] { delete[] output_ops; });
//
//   for (int i = 0; i < n_inputs; ++i) {
//     input_ops[i] = TF_OutputOperation(inputs[i]);
//   }
//   for (int i = 0; i < n_outputs; ++i) {
//     output_ops[i] = TF_OutputOperation(outputs[i]);
//   }
//
//   model_meta_.input_ops.assign(input_ops, input_ops + n_inputs);
//   model_meta_.output_ops.assign(output_ops, output_ops + n_outputs);
// }

}  // namespace infer_engine
