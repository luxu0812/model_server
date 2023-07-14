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
  session_(nullptr),
  graph_(nullptr),
  graph_buffer_(nullptr) {
  LOG(INFO) << "TFEngine::TFEngine()";
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

void TFEngine::infer() {
  run_session();
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

void TFEngine::create_session() {
  TF_Status *tf_status = TF_NewStatus();
  TF_SessionOptions *tf_session_opts = TF_NewSessionOptions();
  ScopeExitTask delete_tf_status([&tf_status, &tf_session_opts]() {
     TF_DeleteStatus(tf_status);
     TF_DeleteSessionOptions(tf_session_opts);
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
  TF_SetConfig(tf_session_opts, tf_session_conf_str.data(), tf_session_conf_str.size(), tf_status);
  if (TF_GetCode(tf_status) != TF_OK) {
    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
      + model_spec_.brief() + "] " + "Failed to set session config: " + std::string(TF_Message(tf_status));
    throw std::runtime_error(err_msg);
  }

  session_ = TF_NewSession(graph_, tf_session_opts, tf_status);
  if (TF_GetCode(tf_status) != TF_OK) {
    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
      + model_spec_.brief() + "] " + "Failed to create session: " + std::string(TF_Message(tf_status));
    throw std::runtime_error(err_msg);
  }

  LOG(INFO) << "[" << model_spec_.brief() << "] Session created";
}

}  // namespace infer_engine
