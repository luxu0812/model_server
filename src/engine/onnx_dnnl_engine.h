// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_ENGINE_ONNX_DNNL_ENGINE_H_
#define INFER_ENGINE_SRC_ENGINE_ONNX_DNNL_ENGINE_H_

#include <string>
#include "onnxruntime/core/session/onnxruntime_cxx_api.h"
#include "infer_engine/src/engine/onnx_engine.h"

namespace infer_engine {

class ONNXDNNLEngine : public ONNXEngine {
 public:
  ONNXDNNLEngine(const ModelSpec& model_spec, const RuntimeConf& runtime_conf);
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

}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_ENGINE_ONNX_DNNL_ENGINE_H_
