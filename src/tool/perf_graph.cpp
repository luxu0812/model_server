// Copyright 2023 zh.luxu1986@gmail.com

#include <stdint.h>
#include <exception>
#include <vector>
#include <random>
#include <algorithm>

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "BShoshany/BS_thread_pool.hpp"

#include "infer_engine/src/util/process/process_initiator.h"
#include "infer_engine/src/util/functional/timer.h"
#include "infer_engine/src/data/model_spec.h"
#include "infer_engine/src/data/runtime_conf.h"
#include "infer_engine/src/data/type.h"
#include "infer_engine/src/engine/engine.h"
#include "infer_engine/src/engine/tf_engine.h"
#include "infer_engine/src/engine/onnx_engine.h"

DEFINE_uint32(concurrency, 1, "Number of concurrent workers");
DEFINE_uint32(batch_size, 128, "Batch size");
DEFINE_uint32(test_data_size, 10000, "Test data size");
DEFINE_int32(opt_level, 0, "Optimization level");
DEFINE_int32(jit_level, 0, "JIT level");
DEFINE_int32(inter_op_parallelism_threads, 1, "Inter op parallelism threads");
DEFINE_int32(intra_op_parallelism_threads, 1, "Intra op parallelism threads");
DEFINE_string(engine_brand, "TensorFlow", "Engine brand");

infer_engine::Engine *create_engine();
std::vector<infer_engine::Sample> *create_samples();
void infer(infer_engine::Engine *engine, infer_engine::Sample *sample, double *cost_ms);

int main(int argc, char **argv) {
  infer_engine::init(argc, argv);
  try {
    std::unique_ptr<infer_engine::Engine> engine(create_engine());
    std::unique_ptr<std::vector<infer_engine::Sample>> samples(create_samples());
    std::vector<double> cost_ms(samples->size());

    BS::thread_pool works(FLAGS_concurrency);
    infer_engine::Timer timer;
    for (int32_t i = 0; i < samples->size(); ++i) {
      works.push_task(infer, engine.get(), &(samples->at(i)), &(cost_ms[i]));
    }
    works.wait_for_tasks();
    double total_cost_sec = timer.f64_elapsed_sec();

    std::sort(cost_ms.begin(), cost_ms.end());
    double cost_avg = std::accumulate(cost_ms.begin(), cost_ms.end(), 0.0) / cost_ms.size();
    double cost_p99 = cost_ms[static_cast<int32_t>(cost_ms.size() * 0.99)];
    LOG(INFO) << "Total cost: " << total_cost_sec << " sec, avg cost: "
              << cost_avg << " ms, p99 cost: " << cost_p99 << " ms";
  } catch (const std::exception& e) {
    LOG(ERROR) << e.what();
  } catch (...) {
    LOG(ERROR) << "Unknown exception";
  }

  return 0;
}

infer_engine::Engine *create_engine() {
  infer_engine::RuntimeConf runtime_conf {
    .opt_level = FLAGS_opt_level,
    .jit_level = FLAGS_jit_level,
    .inter_op_parallelism_threads = FLAGS_inter_op_parallelism_threads,
    .intra_op_parallelism_threads = FLAGS_intra_op_parallelism_threads
  };
  infer_engine::ModelSpec model_spec {
    .name = "test",
    .version = "1.0.0",
    .meta_file = "test/data/model1/graph_meta.json"
  };

  if (FLAGS_engine_brand == "TensorFlow") {
    model_spec.graph_file = "test/data/model1/graph.pb";
    return new infer_engine::TFEngine(model_spec, runtime_conf);
  } else if (FLAGS_engine_brand == "ONNX") {
    model_spec.graph_file = "test/data/model1/graph.onnx";
    return new infer_engine::ONNXEngine(model_spec, runtime_conf);
  } else {
    throw std::runtime_error("Unknown engine brand");
  }

  return nullptr;
}

std::vector<infer_engine::Sample> *create_samples() {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::string meta_file = "test/data/model1/graph_meta.json";
  infer_engine::ModelMeta model_meta;
  model_meta.load(meta_file);

  std::vector<infer_engine::Sample> *samples = new std::vector<infer_engine::Sample>();
  samples->resize(FLAGS_test_data_size);
  for (auto& sample : *samples) {
    sample.instance.batch_size = FLAGS_batch_size;

    sample.instance.features.resize(model_meta.input_shapes.size());
    int32_t i = 0;
    for (auto& input : model_meta.input_shapes) {
      auto& feature = sample.instance.features[i++];
      int64_t data_size = FLAGS_batch_size;
      for (auto& dim : input.second) {
        data_size *= dim;
      }
      feature.name = input.first;
      feature.data.resize(data_size);

      // fill random data
      for (auto& data : feature.data) {
        data = std::uniform_real_distribution<float>(0.0, 1.0)(gen);
      }
    }

    sample.score.targets.resize(model_meta.output_shapes.size());
  }

  return samples;
}

void infer(infer_engine::Engine *engine, infer_engine::Sample *sample, double *cost_ms) {
  infer_engine::Timer timer;
  infer_engine::ScopeExitTask scope_exit_task([&]() {
    *cost_ms = timer.f64_elapsed_ms();
  });

  try {
    engine->infer(&sample->instance, &sample->score);
  } catch (const std::exception& e) {
    LOG(ERROR) << e.what();
  } catch (...) {
    LOG(ERROR) << "Unknown exception";
  }
}
