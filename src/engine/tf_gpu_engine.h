// Copyright (C) 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_ENGINE_TF_GPU_ENGINE_H_
#define MODEL_SERVER_SRC_ENGINE_TF_GPU_ENGINE_H_

#include <memory>
#include <string>
#include "tensorflow/c/c_api.h"
#include "model_server/src/engine/tf_engine.h"

namespace model_server {

class TFGPUEngine : public TFEngine {
 public:
  explicit TFGPUEngine(const EngineConf& engine_conf) noexcept(false);
  virtual ~TFGPUEngine();

  TFGPUEngine() = delete;
  TFGPUEngine& operator=(const TFGPUEngine&) = delete;
  TFGPUEngine(const TFGPUEngine&) = delete;

  // Get brand of engine
  std::string brand() noexcept override;

 protected:
  // Set session options
  void set_gpu(tensorflow::ConfigProto *tf_session_conf) noexcept(false) override;
};

class TFGPUEngineFactory : public EngineFactory {
 private:
  static std::unique_ptr<TFGPUEngineFactory> instance_;

 protected:
  TFGPUEngineFactory() = default;

 public:
  TFGPUEngineFactory(const TFGPUEngineFactory&) = delete;
  TFGPUEngineFactory& operator=(const TFGPUEngineFactory&) = delete;

  TFGPUEngineFactory(TFGPUEngineFactory&&) = delete;
  TFGPUEngineFactory& operator=(TFGPUEngineFactory&&) = delete;

  virtual ~TFGPUEngineFactory() {}

  static EngineFactory *instance();

  virtual Engine *create(const EngineConf& engine_conf) noexcept(false) {
    Engine *engine = new TFGPUEngine(engine_conf);
    engine->init();
    return engine;
  }
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_ENGINE_TF_GPU_ENGINE_H_
