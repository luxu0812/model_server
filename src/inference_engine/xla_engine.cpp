// Copyright 2023 zh.luxu1986@gmail.com

#include "infer_engine/src/inference_engine/xla_engine.h"

namespace infer_engine {

XLAEngine::XLAEngine() : session_(nullptr) {}

XLAEngine::~XLAEngine() {
  destroy();
}

ReturnStatus XLAEngine::init() {
  // Initialize XLA runtime specific resources
  // e.g., Load XLA-specific libraries, initialize XLA context

  // Create a TensorFlow session using XLA runtime
  tensorflow::SessionOptions options;
  tensorflow::Status status = tensorflow::NewSession(options, &session_);
  if (!status.ok()) {
    return ReturnStatus(status.error_message());
  }

  return ReturnStatus::success();
}

ReturnStatus XLAEngine::destroy() {
  // Release any resources associated with XLA runtime

  if (session_ != nullptr) {
    tensorflow::Status status = session_->Close();
    if (!status.ok()) {
      return ReturnStatus(status.error_message());
    }
    delete session_;
    session_ = nullptr;
  }

  return ReturnStatus::success();
}

ReturnStatus XLAEngine::infer() {
  // Perform inference using the XLA runtime

  // Load the TensorFlow model from the .pb file
  tensorflow::GraphDef graph_def;
  tensorflow::Status status = tensorflow::ReadBinaryProto(
    tensorflow::Env::Default(), "/path/to/model.pb", &graph_def
  );  // NOLINT
  if (!status.ok()) {
    return ReturnStatus(status.error_message());
  }

  // Add the graph to the TensorFlow session
  status = session_->Create(graph_def);
  if (!status.ok()) {
    return ReturnStatus(status.error_message());
  }

  return ReturnStatus::success();
}

}  // namespace infer_engine
