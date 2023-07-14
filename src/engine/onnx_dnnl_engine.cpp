// Copyright 2023 zh.luxu1986@gmail.com

#include <vector>
#include "glog/logging.h"
#include "infer_engine/src/engine/onnx_dnnl_engine.h"

namespace infer_engine {

ONNXDNNLEngine::ONNXDNNLEngine(const ModelSpec& model_spec) : ONNXEngine(model_spec) {}

ONNXDNNLEngine::~ONNXDNNLEngine() {}

std::string ONNXDNNLEngine::brand() {
  return kBrandONNXDNNL;
}

void ONNXDNNLEngine::set_session_options() {
  // set session options
  env_ = new Ort::Env(OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING, "orttest");

  session_opts_ = new Ort::SessionOptions();
  session_opts_->SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
  session_opts_->SetIntraOpNumThreads(1);
  session_opts_->SetInterOpNumThreads(1);
  // session_opts_->DisablePerSessionThreads();

  bool enable_cpu_mem_arena = true;
  Ort::ThrowOnError(OrtSessionOptionsAppendExecutionProvider_Dnnl(session_opts_, enable_cpu_mem_arena));
  session_opts_.SetExecutionMode(ExecutionMode::ORT_PARALLEL);

  LOG(INFO) << "[" << model_spec_.brief() << "] Session options set";
}

}  // namespace infer_engine
