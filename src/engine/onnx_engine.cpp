// Copyright 2023 zh.luxu1986@gmail.com

#include <vector>
#include "glog/logging.h"
#include "infer_engine/src/engine/onnx_engine.h"

namespace infer_engine {

ONNXEngine::ONNXEngine(const ModelSpec& model_spec) :
  Engine(model_spec),
  env_(nullptr),
  session_(nullptr) {
  init();
}

ONNXEngine::~ONNXEngine() {
  try {
    if (nullptr != session_) {
      delete session_;
      session_ = nullptr;
      LOG(INFO) << "[" << model_spec_.brief() << "] Session deleted";
    }

    if (nullptr != env_) {
      delete env_;
      env_ = nullptr;
      LOG(INFO) << "[" << model_spec_.brief() << "] Env deleted";
    }
  } catch (const std::exception& e) {
    LOG(ERROR) << e.what();
  } catch (...) {
    LOG(ERROR) << "Unknown exception";
  }
}

std::string ONNXEngine::brand() {
  return kBrandONNX;
}

void ONNXEngine::infer() {
  // Perform inference using the ONNX runtime

  // // Prepare input tensors
  // std::vector<const char*> input_names = session_->GetInputNames();
  // std::vector<Ort::Value> input_tensors(input_names.size());

  // // Set input data to the input tensors
  // // ...

  // // Prepare output tensors
  // std::vector<const char*> output_names = session_->GetOutputNames();
  // std::vector<Ort::Value> output_tensors(output_names.size());

  // // Run inference using the ONNX runtime
  // Ort::RunOptions run_options;
  // Ort::Status status = session_->Run(
  //   run_options,
  //   input_names.data(), input_tensors.data(), input_names.size(),
  //   output_names.data(), output_tensors.data(), output_names.size()
  // );  // NOLINT
  // if (status != Ort::Status::OK()) {
  //   return true;
  // }

  // // Process the output tensors
  // // ...
}

void ONNXEngine::trace() {
}

void ONNXEngine::load() {
  // Load the TensorFlow graph from the .pb file
}

void ONNXEngine::build() {
  // build engine
}

void ONNXEngine::create_session() {
  // create session
  env_ = new Ort::Env(OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING, "orttest");
  Ort::SessionOptions onnx_session_opts;
  onnx_session_opts.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
  // onnx_session_opts.DisablePerSessionThreads();
  onnx_session_opts.SetIntraOpNumThreads(1);
  onnx_session_opts.SetInterOpNumThreads(1);

  session_ = new Ort::Session(*env_, model_spec_.graph_file.c_str(), onnx_session_opts);
  LOG(INFO) << "[" << model_spec_.brief() << "] Session created";
}

}  // namespace infer_engine
