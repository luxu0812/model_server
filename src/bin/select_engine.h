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
#ifdef USE_TF_GPU_ENGINE
#include "model_server/src/engine/tf_gpu_engine.h"
#endif
#ifdef USE_ONNXRUNTIME_ENGINE
#include "model_server/src/engine/onnx_engine.h"
#endif
#ifdef USE_ONNXRUNTIME_DNNL_ENGINE
#include "model_server/src/engine/onnx_dnnl_engine.h"
#endif

model_server::Engine *select_engine(const model_server::EngineConf& engine_conf) {
  model_server::EngineFactory *factory = nullptr;
#ifdef USE_TF_ENGINE
  factory = model_server::TFEngineFactory::instance();
#endif
#ifdef USE_TF_GPU_ENGINE
  factory = model_server::TFGPUEngineFactory::instance();
#endif
#ifdef USE_ONNXRUNTIME_ENGINE
  factory = model_server::ONNXEngineFactory::instance();
#endif
#ifdef USE_ONNXRUNTIME_DNNL_ENGINE
  factory = model_server::ONNXDNNLEngineFactory::instance();
#endif

  if (nullptr != factory) {
    return factory->create(engine_conf);
  }
  return nullptr;
}

model_server::Engine *create_demo_engine_2() {
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
#ifdef USE_TF_GPU_ENGINE
  engine_conf.graph_file_loc = "data/models/model_2/1/graph.pb";
#endif
#ifdef USE_ONNXRUNTIME_ENGINE
  engine_conf.graph_file_loc = "data/models/model_2/1/graph.onnx";
#endif
#ifdef USE_ONNXRUNTIME_DNNL_ENGINE
  engine_conf.graph_file_loc = "data/models/model_2/1/graph.onnx";
#endif

  return select_engine(engine_conf);
}

model_server::Engine *create_demo_engine_3() {
  int32_t cpu_core_num = static_cast<int32_t>(std::thread::hardware_concurrency() >> 1);
  if (0 == cpu_core_num) {
    cpu_core_num = 1;
  }
  model_server::EngineConf engine_conf {
    .name = "model_3",
    .version = "1.0.0",
    .input_nodes = {"sparse_input_unfolded"},
    .output_nodes = {
      "atc_click",
      "enhanced_card_click",
      "pdp_buynow_click",
      "video_anchor_click",
      "enhanced_card_impression",
      "shop_bag_click",
      "pdp_atc_click",
      "buynow_click"
    }, // NOLINT
    .opt_level = FLAGS_engine_opt_level,
    .jit_level = FLAGS_engine_jit_level,
    .inter_op_parallelism_threads = cpu_core_num,
    .intra_op_parallelism_threads = cpu_core_num,
    .use_global_thread_pool = false,
    .ort_parrallel_execution = false
  };

#ifdef USE_TF_ENGINE
  engine_conf.graph_file_loc = "data/models/model_3/3/graph.pb";
#endif
#ifdef USE_TF_GPU_ENGINE
  engine_conf.graph_file_loc = "data/models/model_3/3/graph.pb";
#endif
#ifdef USE_ONNXRUNTIME_ENGINE
  engine_conf.graph_file_loc = "data/models/model_3/3/graph.onnx";
#endif
#ifdef USE_ONNXRUNTIME_DNNL_ENGINE
  engine_conf.graph_file_loc = "data/models/model_3/3/graph.onnx";
#endif

  return select_engine(engine_conf);
}

#endif  // MODEL_SERVER_SRC_BIN_SELECT_ENGINE_H_
