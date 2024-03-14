// Copyright (C) 2023 zh.luxu1986@gmail.com

#include "model_server/src/engine/tf_gpu_engine.h"

#include <fstream>
#include <string>

#include "absl/log/log.h"
#include "tensorflow/c/c_api.h"
#include "tensorflow/core/protobuf/config.pb.h"

namespace model_server {

TFGPUEngine::TFGPUEngine(const EngineConf& engine_conf) noexcept(false) :
  TFEngine(engine_conf) {
}

TFGPUEngine::~TFGPUEngine() {}

std::string TFGPUEngine::brand() noexcept {
  return kBrandTFGPU;
}

void TFGPUEngine::set_gpu(tensorflow::ConfigProto *tf_session_conf) noexcept(false) {
  if (nullptr == tf_session_conf) {
    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "] "
      + "Session config is nullptr";
    throw std::runtime_error(err_msg);
  }

  // (*tf_session_conf.mutable_device_count())["GPU"] = 1;
  tensorflow::GPUOptions gpu;
  gpu.set_per_process_gpu_memory_fraction(0.0);
  gpu.set_allow_growth(true);
  tf_session_conf->mutable_gpu_options()->CopyFrom(gpu);
}

std::unique_ptr<TFGPUEngineFactory> TFGPUEngineFactory::instance_ = nullptr;
EngineFactory *TFGPUEngineFactory::instance() {
  if (nullptr == instance_) {
    instance_.reset(new TFGPUEngineFactory());
  }
  return instance_.get();
}

}  // namespace model_server
