// Copyright (C) 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_ENGINE_ONNX_TVM_ENGINE_H_
#define MODEL_SERVER_SRC_ENGINE_ONNX_TVM_ENGINE_H_

#include <string>
#include "onnxruntime/core/session/onnxruntime_cxx_api.h"
#include "model_server/src/engine/onnx_engine.h"

namespace model_server {

class ONNXTVMEngine : public ONNXEngine {
 public:
  explicit ONNXTVMEngine(const EngineConf& engine_conf) noexcept(false);
  virtual ~ONNXTVMEngine();

  ONNXTVMEngine() = delete;
  ONNXTVMEngine& operator=(const ONNXTVMEngine&) = delete;
  ONNXTVMEngine(const ONNXTVMEngine&) = delete;

  // Get brand of engine
  std::string brand() noexcept override;

 protected:
  // Set session options
  void set_session_options() override;
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_ENGINE_ONNX_TVM_ENGINE_H_
