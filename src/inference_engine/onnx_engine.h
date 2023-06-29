// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_INFERENCE_ENGINE_ONNX_ENGINE_H_
#define INFER_ENGINE_SRC_INFERENCE_ENGINE_ONNX_ENGINE_H_

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

#include "infer_engine/src/data/error_message.h"
#include "infer_engine/src/inference_engine/inference_engine.h"

namespace infer_engine {

class ONNXEngine : public InferenceEngine {
 public:
  ONNXEngine();
  ~ONNXEngine();

  ReturnStatus init() override;
  ReturnStatus destroy() override;

  ReturnStatus infer() override;

 private:
  Ort::Session* session_;
};

}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_INFERENCE_ENGINE_ONNX_ENGINE_H_
