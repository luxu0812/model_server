// Copyright 2023 zh.luxu1986@gmail.com

#include "model_server/src/engine/tf_gpu_engine.h"

#include <fstream>
#include <string>

#include "glog/logging.h"
#include "tensorflow/c/c_api.h"
#include "tensorflow/core/protobuf/config.pb.h"

#include "model_server/src/util/functional/scope_exit_task.h"

namespace model_server {

TFGPUEngine::TFGPUEngine(const EngineConf& engine_conf) noexcept(false) : TFEngine(engine_conf) {}

TFGPUEngine::~TFGPUEngine() {}

std::string TFGPUEngine::brand() noexcept {
  return kBrandTFGPU;
}

//        ::tf_graph::GPUOptions gpu;
//        gpu.set_per_process_gpu_memory_fraction(0.0);
//        gpu.set_allow_growth(true);
//
//        conf.mutable_graph_options()->mutable_optimizer_options()->CopyFrom(optimizer);
//        conf.mutable_gpu_options()->CopyFrom(gpu);


}  // namespace model_server
