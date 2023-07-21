// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_ENGINE_ONNX_ENGINE_H_
#define INFER_ENGINE_SRC_ENGINE_ONNX_ENGINE_H_

#include <string>
#include "onnxruntime/core/session/onnxruntime_cxx_api.h"
#include "infer_engine/src/engine/engine.h"

namespace infer_engine {

class ONNXEngine : public Engine {
 public:
  explicit ONNXEngine(const ModelSpec& model_spec);
  ~ONNXEngine();

  ONNXEngine() = delete;
  ONNXEngine& operator=(const ONNXEngine&) = delete;
  ONNXEngine(const ONNXEngine&) = delete;

  // Get brand of engine
  std::string brand() override;

  // Perform inference using the ONNX runtime
  void infer(BatchInstance *batch_instance, BatchScore *batch_score) override;

  // Perform inference with trace using the ONNX runtime
  void trace() override;

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

}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_ENGINE_ONNX_ENGINE_H_
