// Copyright (C) 2023 zh.luxu1986@gmail.com

#include <stdint.h>
#include <exception>
#include <vector>

#include "glog/logging.h"
#include "BShoshany/BS_thread_pool.hpp"

#include "model_server/src/util/process/process_initiator.h"
#include "model_server/src/util/functional/timer.h"
#include "model_server/src/config/gflags.h"
#include "model_server/src/engine/sample.h"
#include "model_server/src/engine/engine.h"
#include "model_server/src/engine/tf_engine.h"

model_server::Engine *create_engine();

int main(int argc, char **argv) {
  model_server::init(argc, argv);
  try {
    std::unique_ptr<model_server::Engine> engine(create_engine());
    model_server::PerfIndex perf_index;
    engine->perf(FLAGS_number_of_consumers, FLAGS_number_of_test_cases, FLAGS_batch_size, &perf_index);
    LOG(INFO) << "summary:\n" << perf_index.DebugString();
  } catch (const std::exception& e) {
    LOG(ERROR) << e.what();
  } catch (...) {
    LOG(ERROR) << "Unknown exception";
  }

  return 0;
}

model_server::Engine *create_engine() {
  model_server::EngineConf engine_conf {
    .name = "model_2",
    .version = "1.0.0",
    .input_nodes = {"dense", "onehot", "sparse_input_folded", "sparse_input_unfolded"},
    .output_nodes = {"predict_node", "p0_click", "p0_atc", "p0_order"},
    .opt_level = FLAGS_engine_opt_level,
    .jit_level = FLAGS_engine_jit_level,
    .inter_op_parallelism_threads = FLAGS_engine_inter_op_parallelism_threads,
    .intra_op_parallelism_threads = FLAGS_engine_intra_op_parallelism_threads
  };

  engine_conf.graph_file_loc = "data/models/model_2/1/graph.pb";
  return new model_server::TFEngine(engine_conf);
}
