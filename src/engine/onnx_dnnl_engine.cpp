// Copyright 2023 zh.luxu1986@gmail.com

#include <vector>
#include "glog/logging.h"
#include "infer_engine/src/engine/onnx_dnnl_engine.h"

namespace infer_engine {

ONNXDNNLEngine::ONNXDNNLEngine(const ModelSpec& model_spec, const SessionConf& session_conf) :
  ONNXEngine(model_spec, session_conf) {}

ONNXDNNLEngine::~ONNXDNNLEngine() {}

std::string ONNXDNNLEngine::brand() {
  return kBrandONNXDNNL;
}

void ONNXDNNLEngine::set_session_options() {
  ONNXEngine::set_session_options();

  bool enable_cpu_mem_arena = true;
  Ort::ThrowOnError(OrtSessionOptionsAppendExecutionProvider_Dnnl(session_opts_, enable_cpu_mem_arena));
  session_opts_.SetExecutionMode(ExecutionMode::ORT_PARALLEL);

  LOG(INFO) << "[" << model_spec_.brief() << "] Session options set";
}

}  // namespace infer_engine
