// Copyright (C) 2023 zh.luxu1986@gmail.com

#include "model_server/src/engine/onnx_tvm_engine.h"
#include <vector>
#include "glog/logging.h"
#include "onnxruntime/tvm_provider_factory.h"

namespace model_server {

ONNXTVMEngine::ONNXTVMEngine(const EngineConf& engine_conf) noexcept(false) :
  ONNXEngine(engine_conf) {}

ONNXTVMEngine::~ONNXTVMEngine() {}

std::string ONNXTVMEngine::brand() noexcept {
  return kBrandONNXTVM;
}

void ONNXTVMEngine::set_session_options() {
  ONNXEngine::set_session_options();
  Ort::ThrowOnError(OrtSessionOptionsAppendExecutionProvider_Tvm(session_opts_->GetUnowned(), ""));
  LOG(INFO) << "Set TVM as execution provider";
}

}  // namespace model_server
