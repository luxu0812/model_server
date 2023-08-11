// Copyright 2023 zh.luxu1986@gmail.com

#include <vector>
#include "glog/logging.h"
#include "model_server/src/engine/onnx_engine.h"

namespace model_server {

ONNXEngine::ONNXEngine(const EngineConf& engine_conf) noexcept(false) :
  Engine(engine_conf),
  env_(nullptr),
  session_opts_(nullptr),
  session_(nullptr) {
  init();
}

ONNXEngine::~ONNXEngine() {
  try {
    if (nullptr != session_) {
      delete session_;
      session_ = nullptr;
      LOG(INFO) << "[" << conf_.brief() << "] Session deleted";
    }

    if (nullptr != env_) {
      delete env_;
      env_ = nullptr;
      LOG(INFO) << "[" << conf_.brief() << "] Env deleted";
    }

    if (nullptr != session_opts_) {
      delete session_opts_;
      session_opts_ = nullptr;
      LOG(INFO) << "[" << conf_.brief() << "] Session options deleted";
    }
  } catch (const std::exception& e) {
    LOG(ERROR) << e.what();
  } catch (...) {
    LOG(ERROR) << "Unknown exception";
  }
}

std::string ONNXEngine::brand() noexcept {
  return kBrandONNX;
}

  // Perform inference using the ONNX runtime
void ONNXEngine::infer(Instance *instance, Score *score) noexcept(false) {
  // Prepare input tensors
  // std::vector<const char*> input_names = session_->GetInputNames();
  // std::vector<Ort::Value> input_tensors(input_names.size());

  // Set input data to the input tensors
  // ...

  // Prepare output tensors
  // std::vector<const char*> output_names = session_->GetOutputNames();
  // std::vector<Ort::Value> output_tensors(output_names.size());

  // Run inference using the ONNX runtime
  // Ort::RunOptions run_options;
  // Ort::Status status = session_->Run(
  //   run_options,
  //   input_names.data(), input_tensors.data(), input_names.size(),
  //   output_names.data(), output_tensors.data(), output_names.size()
  // );  // NOLINT
  // if (status != Ort::Status::OK()) {
  //   const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
  //     + conf_.brief() + "] " + "Failed to run session: " + status.ErrorMessage();
  //   throw std::runtime_error(err_msg);
  // }

  // Process the output tensors
  // ...
}

void ONNXEngine::trace() noexcept(false) {
}

void ONNXEngine::load() {
  // Load the TensorFlow graph from the .onnx file
}

void ONNXEngine::build() {
  // build engine
}

void ONNXEngine::set_session_options() {
  // set session options

  session_opts_ = new Ort::SessionOptions();
  session_opts_->SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
  session_opts_->SetIntraOpNumThreads(conf_.intra_op_parallelism_threads);
  session_opts_->SetInterOpNumThreads(conf_.inter_op_parallelism_threads);
  // session_opts_->DisablePerSessionThreads();

  LOG(INFO) << "[" << conf_.brief() << "] Session options set";
}

void ONNXEngine::create_session() {
  // create session
  env_ = new Ort::Env(OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING, conf_.name.c_str());
  session_ = new Ort::Session(*env_, conf_.graph_file_loc.c_str(), *session_opts_);
  LOG(INFO) << "[" << conf_.brief() << "] Session created";
}

}  // namespace model_server
