// Copyright (C) 2023 zh.luxu1986@gmail.com

#include "model_server/src/engine/onnx_tvm_engine.h"
#include <vector>
#include "glog/logging.h"
#include "onnxruntime/dnnl_provider_options.h"

namespace model_server {

ONNXTVMEngine::ONNXTVMEngine(const EngineConf& engine_conf) noexcept(false) :
  ONNXEngine(engine_conf) {}

ONNXTVMEngine::~ONNXTVMEngine() {}

std::string ONNXTVMEngine::brand() noexcept {
  return kBrandONNXTVM;
}

}  // namespace model_server
