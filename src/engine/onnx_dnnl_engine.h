// Copyright (C) 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_ENGINE_ONNX_DNNL_ENGINE_H_
#define MODEL_SERVER_SRC_ENGINE_ONNX_DNNL_ENGINE_H_

#include <memory>
#include <string>
#include "onnxruntime/onnxruntime_cxx_api.h"
#include "model_server/src/engine/onnx_engine.h"

namespace model_server {

class ONNXDNNLEngine : public ONNXEngine {
 public:
  explicit ONNXDNNLEngine(const EngineConf& engine_conf) noexcept(false);
  virtual ~ONNXDNNLEngine();

  ONNXDNNLEngine() = delete;
  ONNXDNNLEngine& operator=(const ONNXDNNLEngine&) = delete;
  ONNXDNNLEngine(const ONNXDNNLEngine&) = delete;

  // Get brand of engine
  std::string brand() noexcept override;

 protected:
  // Set session options
  void set_session_options() override;
};

class ONNXDNNLEngineFactory : public EngineFactory {
 private:
  static std::unique_ptr<ONNXDNNLEngineFactory> instance_;

 protected:
  ONNXDNNLEngineFactory() = default;

 public:
  ONNXDNNLEngineFactory(const ONNXDNNLEngineFactory&) = delete;
  ONNXDNNLEngineFactory& operator=(const ONNXDNNLEngineFactory&) = delete;

  ONNXDNNLEngineFactory(ONNXDNNLEngineFactory&&) = delete;
  ONNXDNNLEngineFactory& operator=(ONNXDNNLEngineFactory&&) = delete;

  virtual ~ONNXDNNLEngineFactory() {}

  static EngineFactory *instance();

  virtual Engine *create(const EngineConf& engine_conf) noexcept(false) {
    Engine *engine = new ONNXDNNLEngine(engine_conf);
    engine->init();
    return engine;
  }
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_ENGINE_ONNX_DNNL_ENGINE_H_
