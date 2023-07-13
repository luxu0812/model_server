// Copyright 2023 zh.luxu1986@gmail.com

#include <vector>

#include "infer_engine/src/engine/onnx_engine.h"

namespace infer_engine {

ONNXEngine::ONNXEngine(const ModelSpec& model_spec) : Engine(model_spec), session_(nullptr) {
  init();
}

ONNXEngine::~ONNXEngine() {
}

// void ONNXEngine::init() {
  // Initialize ONNX runtime specific resources
  // e.g., Load ONNX runtime libraries, initialize ONNX context

  // Create an ONNX session
  // Ort::SessionOptions session_options;
  // session_ = new Ort::Session(
  //   Ort::Env(ORT_LOGGING_LEVEL_WARNING, "ONNXEngine"), "/path/to/model.onnx", session_options
  // );  // NOLINT
// }

// void ONNXEngine::destroy() {
  // Release any resources associated with ONNX runtime

  // if (session_ != nullptr) {
  //   delete session_;
  //   session_ = nullptr;
  // }
// }

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

}  // namespace infer_engine
