// Copyright (C) 2023 zh.luxu1986@gmail.com

#include "model_server/src/engine/onnx_dnnl_engine.h"
#include <vector>
#include "glog/logging.h"
#include "onnxruntime/dnnl_provider_options.h"

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
  session_opts_->AppendExecutionProvider_Dnnl(dnnl_options);
  LOG(INFO) << "Set DNNL as execution provider";
}

}  // namespace model_server
