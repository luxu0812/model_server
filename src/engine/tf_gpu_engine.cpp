// Copyright 2023 zh.luxu1986@gmail.com

#include "infer_engine/src/engine/tf_gpu_engine.h"

#include <fstream>
#include <string>

#include "glog/logging.h"
#include "tensorflow/c/c_api.h"
#include "tensorflow/core/protobuf/config.pb.h"

#include "infer_engine/src/util/functional/scope_exit_task.h"

namespace infer_engine {

TFGPUEngine::TFGPUEngine(const ModelSpec& model_spec, const SessionConf& session_conf) :
  TFEngine(model_spec, session_conf) {}

TFGPUEngine::~TFGPUEngine() {}

std::string TFGPUEngine::brand() {
  return kBrandTFGPU;
}

}  // namespace infer_engine
