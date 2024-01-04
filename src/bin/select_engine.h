// Copyright (C) 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_BIN_SELECT_ENGINE_H_
#define MODEL_SERVER_SRC_BIN_SELECT_ENGINE_H_

#include <string>
#include "model_server/src/engine/engine.h"

#ifdef USE_TF_ENGINE
#include "model_server/src/engine/tf_engine.h"
#endif
#ifdef USE_ONNXRUNTIME_ENGINE
#include "model_server/src/engine/onnx_engine.h"
#endif
#ifdef USE_ONNXRUNTIME_DNNL_ENGINE
#include "model_server/src/engine/onnx_dnnl_engine.h"
#endif
#ifdef USE_ONNXRUNTIME_TVM_ENGINE
#include "model_server/src/engin/onnx_tvm_engine.h"
#endif

model_server::Engine *select_engine(const model_server::EngineConf& engine_conf) {
#ifdef USE_TF_ENGINE
  return new model_server::TFEngine(engine_conf);
#endif
#ifdef USE_ONNXRUNTIME_ENGINE
  return new model_server::ONNXEngine(engine_conf);
#endif
#ifdef USE_ONNXRUNTIME_DNNL_ENGINE
  return new model_server::ONNXDNNLEngine(engine_conf);
#endif
#ifdef USE_ONNXRUNTIME_TVM_ENGINE
  return new model_server::ONNXTVMEngine(engine_conf);
#endif
}

#endif  // MODEL_SERVER_SRC_BIN_SELECT_ENGINE_H_
