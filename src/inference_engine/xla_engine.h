// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_INFERENCE_ENGINE_XLA_ENGINE_H_
#define INFER_ENGINE_SRC_INFERENCE_ENGINE_XLA_ENGINE_H_

#include "tensorflow/core/public/session.h"
#include "tensorflow/core/protobuf/meta_graph.pb.h"

#include "infer_engine/src/data/return_status.h"
#include "infer_engine/src/inference_engine/inference_engine.h"

namespace infer_engine {

class XLAEngine : public InferenceEngine {
 public:
  XLAEngine();
  ~XLAEngine();

  bool init() override;
  bool destroy() override;

  bool infer() override;

 private:
  tensorflow::Session* session_;
};

}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_INFERENCE_ENGINE_XLA_ENGINE_H_
