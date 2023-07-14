// Copyright 2023 zh.luxu1986@gmail.com

#include "infer_engine/src/engine/tf_engine.h"

#include <iostream>
#include <fstream>
#include <string>

#include "glog/logging.h"
#include "tensorflow/c/c_api.h"
#include "tensorflow/core/protobuf/config.pb.h"

#include "infer_engine/src/util/functional/scope_exit_task.h"

namespace infer_engine {

TFEngine::TFEngine(const ModelSpec& model_spec) :
  Engine(model_spec),
  session_(),
  graph_(),
  graph_buffer_() {
  init();
}

TFEngine::~TFEngine() {
  destroy();
}

void TFEngine::infer() {
  // Perform inference using the TF runtime

  // Load the TensorFlow graph from the .pb file
  // tensorflow::GraphDef graph_def;
  // tensorflow::Status status = tensorflow::ReadBinaryProto(
  //   tensorflow::Env::Default(), "/path/to/graph.pb", &graph_def
  // );  // NOLINT
  // if (!status.ok()) {
  //   return bool(status.error_message());
  // }

  // Add the graph to the TensorFlow session
  // status = session_->Create(graph_def);
  // if (!status.ok()) {
  //   return bool(status.error_message());
  // }
}

void TFEngine::load_graph() {
  const std::string& graph_file = model_spec_.graph_file;
  std::ifstream file(graph_file, std::ios::binary | std::ios::ate);

  if (!file.is_open()) {
    const std::string& err_msg = "Failed to open graph file: " + graph_file;
    LOG(ERROR) << err_msg;
    throw std::runtime_error(err_msg);
  }
  ScopeExitTask close_file([&file]() { file.close(); });

  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  char *buffer_data = new char[size];
  if (!file.read(buffer_data, size)) {
    delete[] buffer_data;

    const std::string& err_msg = "Failed to read graph file: " + graph_file;
    LOG(ERROR) << err_msg;
    throw std::runtime_error(err_msg);
  }

  graph_buffer_ = new TF_Buffer();
  graph_buffer_->data = buffer_data;
  graph_buffer_->length = size;
  graph_buffer_->data_deallocator = [](void *data, size_t length) {
    delete[] static_cast<char*>(data);
  };
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
    const std::string& err_msg = "Failed to import graph: " + std::string(TF_Message(tf_status));
    LOG(ERROR) << err_msg;
    throw std::runtime_error(err_msg);
  }
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
    const std::string& err_msg = "Failed to set session config: " + std::string(TF_Message(tf_status));
    LOG(ERROR) << err_msg;
    throw std::runtime_error(err_msg);
  }

  session_ = TF_NewSession(graph_, tf_session_opts, tf_status);
  if (TF_GetCode(tf_status) != TF_OK) {
    const std::string& err_msg = "Failed to create sessoin: " + std::string(TF_Message(tf_status));
    LOG(ERROR) << err_msg;
    throw std::runtime_error(err_msg);
  }
}

}  // namespace infer_engine
