// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_ENGINE_TF_GPU_ENGINE_H_
#define INFER_ENGINE_SRC_ENGINE_TF_GPU_ENGINE_H_

#include <string>
#include "tensorflow/c/c_api.h"
#include "infer_engine/src/engine/tf_engine.h"

namespace infer_engine {

class TFGPUEngine : public TFEngine {
 public:
  TFGPUEngine(const ModelSpec& model_spec, const RuntimeConf& runtime_conf);
  ~TFGPUEngine();

  TFGPUEngine() = delete;
  TFGPUEngine& operator=(const TFGPUEngine&) = delete;
  TFGPUEngine(const TFGPUEngine&) = delete;

  // Get brand of engine
  std::string brand() override;

 protected:
  // Set session options
  void set_session_options() override;
};

}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_ENGINE_TF_GPU_ENGINE_H_
