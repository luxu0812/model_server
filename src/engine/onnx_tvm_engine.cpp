// Copyright (C) 2023 zh.luxu1986@gmail.com

#include <vector>
#include "glog/logging.h"
#include "onnxruntime/dnnl_provider_options.h"
#include "model_server/src/engine/onnx_dnnl_engine.h"

namespace model_server {

ONNXTVMEngine::ONNXTVMEngine(const EngineConf& engine_conf) noexcept(false) :
  ONNXEngine(engine_conf) {}

ONNXTVMEngine::~ONNXTVMEngine() {}

std::string ONNXTVMEngine::brand() noexcept {
  return kBrandONNXTVM;
}

void ONNXTVMEngine::set_session_options() {
  ONNXEngine::set_session_options();

  OrtDnnlProviderOptions dnnl_options = {
    .use_arena = true,
    .threadpool_args = nullptr
  };

  session_opts_->AppendExecutionProvider_Dnnl(dnnl_options);

  LOG(INFO) << "[" << conf_.brief() << "] Session options set";
}

}  // namespace model_server
