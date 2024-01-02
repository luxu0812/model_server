// Copyright (C) 2023 zh.luxu1986@gmail.com

#include <vector>
#include "glog/logging.h"
#include "onnxruntime/dnnl_provider_options.h"
#include "model_server/src/engine/onnx_dnnl_engine.h"

namespace model_server {

ONNXDNNLEngine::ONNXDNNLEngine(const EngineConf& engine_conf) noexcept(false) :
  ONNXEngine(engine_conf) {}

ONNXDNNLEngine::~ONNXDNNLEngine() {}

std::string ONNXDNNLEngine::brand() noexcept {
  return kBrandONNXDNNL;
}

void ONNXDNNLEngine::set_session_options() {
  ONNXEngine::set_session_options();

  OrtDnnlProviderOptions dnnl_options = {
    .use_arena = true,
    .threadpool_args = nullptr
  };

  // Ort::ThrowOnError(OrtSessionOptionsAppendExecutionProvider_Dnnl(session_opts_, enable_cpu_mem_arena));
  Ort::ThrowOnError(session_opts_->AppendExecutionProvider_Dnnl(dnnl_options));
  session_opts_->SetExecutionMode(ExecutionMode::ORT_PARALLEL);

  LOG(INFO) << "[" << conf_.brief() << "] Session options set";
}

}  // namespace model_server
