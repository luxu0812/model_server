// Copyright 2023 zh.luxu1986@gmail.com

#include <stdint.h>
#include <exception>
#include <vector>

#include "gflags/gflags.h"
#include "glog/logging.h"

#include "infer_engine/src/util/functional/random.h"
#include "infer_engine/src/util/process/process_initiator.h"
#include "infer_engine/src/data/model_spec.h"
#include "infer_engine/src/engine/engine.h"
#include "infer_engine/src/engine/tf_engine.h"
#include "infer_engine/src/engine/onnx_engine.h"

DEFINE_uint32(concurrency, 1, "Number of concurrent workers");
DEFINE_uint32(batch_size, 128, "Batch size");
DEFINE_uint32(test_data_size, 10000, "Test data size");
DEFINE_string(engine_brand, "TensorFlow", "Engine brand");

infer_engine::Engine *get_engine();
std::vector<infer_engine::Sample> *get_samples();

int main(int argc, char **argv) {
  infer_engine::init(argc, argv);

  try {
    std::unique_ptr<infer_engine::Engine> engine(get_engine());
    std::unique_ptr<std::vector<infer_engine::Sample>> samples(get_samples());
  } catch (const std::exception& e) {
    LOG(ERROR) << e.what();
  } catch (...) {
    LOG(ERROR) << "Unknown exception";
  }

  return 0;
}

infer_engine::Engine *create_engine() {
  if (FLAGS_engine_brand == "TensorFlow") {
    infer_engine::ModelSpec tf_model_spec {
      .name = "test",
      .version = "1.0.0",
      .graph_file = "test/data/model1/graph.pb",
      .meta_file = "test/data/model1/graph_meta.json"
    };
    return new infer_engine::TFEngine(tf_model_spec);
  } else if (FLAGS_engine_brand == "ONNX") {
    infer_engine::ModelSpec onnx_model_spec {
      .name = "test",
      .version = "1.0.0",
      .graph_file = "test/data/model1/graph.onnx",
      .meta_file = "test/data/model1/graph_meta.json"
    };
    return new infer_engine::ONNXEngine(onnx_model_spec);
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
      int64_t data_bytes = FLAGS_batch_size * sizeof(feature.data[0]);
      for (auto& dim : input.second) {
        data_bytes *= dim;
      }
      feature.data.resize(data_bytes);

      // fill random data
      for (auto& data : feature.data) {
        data = std::uniform_real_distribution<float>(0.0, 1.0)(gen);
      }
    }

    sample.score.targets.resize(model_meta.output_shapes.size());
  }

  return samples;
}
