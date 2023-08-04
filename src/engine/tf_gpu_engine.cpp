// Copyright 2023 zh.luxu1986@gmail.com

#include "model_server/src/engine/tf_gpu_engine.h"

#include <fstream>
#include <string>

#include "glog/logging.h"
#include "tensorflow/c/c_api.h"
#include "tensorflow/core/protobuf/config.pb.h"

#include "model_server/src/util/functional/scope_exit_task.h"

namespace model_server {

TFGPUEngine::TFGPUEngine(const EngineConf& engine_conf) : TFEngine(engine_conf) {}

TFGPUEngine::~TFGPUEngine() {}

std::string TFGPUEngine::brand() {
  return kBrandTFGPU;
}

}  // namespace model_server
