// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_INFERENCE_ENGINE_INFERENCE_ENGINE_H_
#define INFER_ENGINE_SRC_INFERENCE_ENGINE_INFERENCE_ENGINE_H_

#include "infer_engine/src/data/return_status.h"

namespace infer_engine {

class InferenceEngine {
 public:
  InferenceEngine();
  ~InferenceEngine();

  virtual ReturnStatus init();
  virtual ReturnStatus destroy();

  virtual ReturnStatus infer();
}

}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_INFERENCE_ENGINE_INFERENCE_ENGINE_H_
