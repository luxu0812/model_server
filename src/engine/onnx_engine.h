// Copyright 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_ENGINE_ONNX_ENGINE_H_
#define MODEL_SERVER_SRC_ENGINE_ONNX_ENGINE_H_

#include <string>
#include "onnxruntime/core/session/onnxruntime_cxx_api.h"
#include "model_server/src/engine/engine.h"

namespace model_server {

class ONNXEngine : public Engine {
 public:
  explicit ONNXEngine(const EngineConf& engine_conf) noexcept(false);
  ~ONNXEngine();

  ONNXEngine() = delete;
  ONNXEngine& operator=(const ONNXEngine&) = delete;
  ONNXEngine(const ONNXEngine&) = delete;

  // Get brand of engine
  std::string brand() noexcept override;

  // Perform inference using the ONNX runtime
  void infer(Instance *instance, Score *score) noexcept(false) override;

  // Perform inference with trace using the ONNX runtime
  void trace() noexcept(false) override;

 protected:
  // Load the TensorFlow graph from the .pb file
  void load() override;

  // Build engine
  void build() override;

  // Set session options
  void set_session_options() override;

  // Create session
  void create_session() override;

 private:
  Ort::Env            *env_;
  Ort::SessionOptions *session_opts_;
  Ort::Session        *session_;
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_ENGINE_ONNX_ENGINE_H_
