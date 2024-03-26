// Copyright (C) 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_ENGINE_TF2_GPU_ENGINE_H_
#define MODEL_SERVER_SRC_ENGINE_TF2_GPU_ENGINE_H_

#include <memory>
#include <string>
#include "model_server/src/engine/tf2_engine.h"

namespace model_server {

class TF2GPUEngine : public TF2Engine {
 public:
  explicit TF2GPUEngine(const EngineConf& engine_conf) noexcept(false);
  virtual ~TF2GPUEngine();

  TF2GPUEngine() = delete;
  TF2GPUEngine& operator=(const TF2GPUEngine&) = delete;
  TF2GPUEngine(const TF2GPUEngine&) = delete;

  // Get brand of engine
  std::string brand() noexcept override;

 protected:
  // Set session options
  void set_gpu(tensorflow::ConfigProto *tf_session_conf) noexcept(false) override;
};

class TF2GPUEngineFactory : public EngineFactory {
 private:
  static std::unique_ptr<TF2GPUEngineFactory> instance_;

 protected:
  TF2GPUEngineFactory() = default;

 public:
  TF2GPUEngineFactory(const TF2GPUEngineFactory&) = delete;
  TF2GPUEngineFactory& operator=(const TF2GPUEngineFactory&) = delete;

  TF2GPUEngineFactory(TF2GPUEngineFactory&&) = delete;
  TF2GPUEngineFactory& operator=(TF2GPUEngineFactory&&) = delete;

  virtual ~TF2GPUEngineFactory() {}

  static EngineFactory *instance();

  virtual Engine *create(const EngineConf& engine_conf) noexcept(false) {
    Engine *engine = new TF2GPUEngine(engine_conf);
    engine->init();
    return engine;
  }
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_ENGINE_TF2_GPU_ENGINE_H_
