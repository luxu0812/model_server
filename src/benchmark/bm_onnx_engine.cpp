// Copyright (C) 2021 zh.luxu1986@gmail.com

#include <vector>
#include "glog/logging.h"
#include "benchmark/benchmark.h"
#include "model_server/src/engine/sample.h"
#include "model_server/src/engine/onnx_engine.h"
#include "model_server/src/population/model_spec.h"
#include "model_server/src/population/sample_gen.h"

const int32_t kTestDataSize = 20;
const int32_t kBatchSize = 128;
static std::vector<model_server::Sample> g_samples;

static void do_setup(const benchmark::State& state) {
  FLAGS_logbufsecs = 0;
  FLAGS_minloglevel = google::ERROR;
  FLAGS_logtostdout = true;

  std::string meta_file = "data/models/model_2/model_conf.json";
  model_server::ModelMeta model_meta;
  model_meta.load(meta_file);
  model_server::random_sample_gen(model_meta, &g_samples, kTestDataSize, kBatchSize);
}

static void do_teardown(const benchmark::State& state) {
}

static void bm_onnx_engine(benchmark::State& state) {  // NOLINT
  model_server::EngineConf engine_conf {
    .name = "model_2",
    .version = "1.0.0",
    .graph_file_loc = "data/models/model_2/1/graph.onnx",
    .input_nodes = {"dense", "onehot", "sparse_input_folded", "sparse_input_unfolded"},
    .output_nodes = {"predict_node", "p0_click", "p0_atc", "p0_order"},
    .opt_level = static_cast<int32_t>(state.range(0)),
    .jit_level = static_cast<int32_t>(state.range(1)),
    .inter_op_parallelism_threads = static_cast<int32_t>(state.range(2)),
    .intra_op_parallelism_threads = static_cast<int32_t>(state.range(3))
  };
  std::unique_ptr<model_server::ONNXEngine> engine(new model_server::ONNXEngine(engine_conf));

  for (auto _ : state) {
    for (auto& sample : g_samples) {
      engine->infer(&sample.instance, &sample.score);
      benchmark::ClobberMemory();
    }
  }
}

BENCHMARK(bm_onnx_engine)
  ->Args({0, 0, 1, 1})
  ->Args({0, 0, 1, 8})
  ->Args({0, 0, 8, 1})
  ->Args({0, 0, 8, 8})
  ->Args({0, 1, 1, 1})
  ->Args({0, 1, 1, 8})
  ->Args({0, 1, 8, 1})
  ->Args({0, 1, 8, 8})
  ->Args({0, 2, 1, 1})
  ->Args({0, 2, 1, 8})
  ->Args({0, 2, 8, 1})
  ->Args({0, 2, 8, 8})
  ->Args({1, 0, 1, 1})
  ->Args({1, 0, 1, 8})
  ->Args({1, 0, 8, 1})
  ->Args({1, 0, 8, 8})
  ->Args({1, 1, 1, 1})
  ->Args({1, 1, 1, 8})
  ->Args({1, 1, 8, 1})
  ->Args({1, 1, 8, 8})
  ->Args({1, 2, 1, 1})
  ->Args({1, 2, 1, 8})
  ->Args({1, 2, 8, 1})
  ->Args({1, 2, 8, 8})
  ->Setup(do_setup)
  ->Teardown(do_teardown)
  ->Unit(benchmark::kMillisecond)
  ->UseRealTime();

BENCHMARK_MAIN();
