// Copyright (C) 2023 zh.luxu1986@gmail.com

#include <stdint.h>
#include <exception>
#include <vector>

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "BShoshany/BS_thread_pool.hpp"

#include "model_server/src/util/process/process_initiator.h"
#include "model_server/src/util/functional/timer.h"
#include "model_server/src/engine/sample.h"
#include "model_server/src/engine/engine.h"
#include "model_server/src/engine/tf_engine.h"
#include "model_server/src/population/model_spec.h"
#include "model_server/src/population/sample_gen.h"
#include "model_server/src/stresstest/stress_framework.h"

DEFINE_uint32(concurrency, 1, "Number of concurrent workers");
DEFINE_uint32(batch_size, 128, "Batch size");
DEFINE_uint32(test_data_size, 1000, "Test data size");
DEFINE_int32(opt_level, 0, "Optimization level");
DEFINE_int32(jit_level, 0, "JIT level");
DEFINE_int32(inter_op_parallelism_threads, 1, "Inter op parallelism threads");
DEFINE_int32(intra_op_parallelism_threads, 1, "Intra op parallelism threads");

model_server::Engine *create_engine();

int main(int argc, char **argv) {
  model_server::init(argc, argv);
  try {
    std::unique_ptr<model_server::Engine> engine(create_engine());
    std::unique_ptr<std::vector<model_server::Sample>> samples(
      engine->random_sample_gen(FLAGS_test_data_size, FLAGS_batch_size)
    );  // NOLINT

    StressFramework stress_framework(engine.get());
    stress_framework.run(samples.get(), FLAGS_concurrency);
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
    .opt_level = FLAGS_opt_level,
    .jit_level = FLAGS_jit_level,
    .inter_op_parallelism_threads = FLAGS_inter_op_parallelism_threads,
    .intra_op_parallelism_threads = FLAGS_intra_op_parallelism_threads
  };

  engine_conf.graph_file_loc = "data/models/model_2/1/graph.pb";
  return new model_server::TFEngine(engine_conf);
}
