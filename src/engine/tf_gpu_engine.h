// Copyright 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_ENGINE_TF_GPU_ENGINE_H_
#define MODEL_SERVER_SRC_ENGINE_TF_GPU_ENGINE_H_

#include <string>
#include "tensorflow/c/c_api.h"
#include "model_server/src/engine/tf_engine.h"

namespace model_server {

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

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_ENGINE_TF_GPU_ENGINE_H_
