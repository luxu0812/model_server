// Copyright (C) 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_BIN_SELECT_ENGINE_H_
#define MODEL_SERVER_SRC_BIN_SELECT_ENGINE_H_

#include <string>
#include <thread>  // NOLINT
#include "model_server/src/engine/engine.h"
#include "model_server/src/config/gflags.h"

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
  return nullptr;
}

model_server::Engine *create_demo_engine() {
  int32_t cpu_core_num = static_cast<int32_t>(std::thread::hardware_concurrency() >> 1);
  if (0 == cpu_core_num) {
    cpu_core_num = 1;
  }
  model_server::EngineConf engine_conf {
    .name = "model_2",
    .version = "1.0.0",
    .input_nodes = {"dense", "onehot", "sparse_input_folded", "sparse_input_unfolded"},
    .output_nodes = {"predict_node", "p0_click", "p0_atc", "p0_order"},
    .opt_level = FLAGS_engine_opt_level,
    .jit_level = FLAGS_engine_jit_level,
    .inter_op_parallelism_threads = cpu_core_num,
    .intra_op_parallelism_threads = cpu_core_num,
    .use_global_thread_pool = false,
    .ort_parrallel_execution = false
  };

#ifdef USE_TF_ENGINE
  engine_conf.graph_file_loc = "data/models/model_2/1/graph.pb";
#endif
#ifdef USE_ONNXRUNTIME_ENGINE
  engine_conf.graph_file_loc = "data/models/model_2/1/graph.onnx";
#endif
#ifdef USE_ONNXRUNTIME_DNNL_ENGINE
  engine_conf.graph_file_loc = "data/models/model_2/1/graph.onnx";
#endif
#ifdef USE_ONNXRUNTIME_TVM_ENGINE
  engine_conf.graph_file_loc = "data/models/model_2/1/graph.onnx";
#endif

  return select_engine(engine_conf);
}

#endif  // MODEL_SERVER_SRC_BIN_SELECT_ENGINE_H_
