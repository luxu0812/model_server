// Copyright 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_ENGINE_ONNX_DNNL_ENGINE_H_
#define MODEL_SERVER_SRC_ENGINE_ONNX_DNNL_ENGINE_H_

#include <string>
#include "onnxruntime/core/session/onnxruntime_cxx_api.h"
#include "model_server/src/engine/onnx_engine.h"

namespace model_server {

class ONNXDNNLEngine : public ONNXEngine {
 public:
  explicit ONNXDNNLEngine(const EngineConf& engine_conf) noexcept(false);
  ~ONNXDNNLEngine();

  ONNXDNNLEngine() = delete;
  ONNXDNNLEngine& operator=(const ONNXDNNLEngine&) = delete;
  ONNXDNNLEngine(const ONNXDNNLEngine&) = delete;

  // Get brand of engine
  std::string brand() override;

 protected:
  // Set session options
  void set_session_options() override;
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_ENGINE_ONNX_DNNL_ENGINE_H_
