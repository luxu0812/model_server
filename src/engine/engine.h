// Copyright (C) 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_ENGINE_ENGINE_H_
#define MODEL_SERVER_SRC_ENGINE_ENGINE_H_

#include <stdint.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "absl/container/flat_hash_map.h"
#include "BShoshany/BS_thread_pool.hpp"
#include "model_server/src/engine/sample.h"

namespace model_server {

const char kBrandTF[]       = "TensorFlow";
const char kBrandTFGPU[]    = "TensorFlow-GPU";
const char kBrandONNX[]     = "ONNX";
const char kBrandONNXDNNL[] = "ONNX-DNNL";

struct EngineConf {
  std::string name                      = "";
  std::string version                   = "";
  std::string graph_file_loc            = "";
  std::vector<std::string> input_nodes  = {};
  std::vector<std::string> output_nodes = {};

  int32_t opt_level                     = 0;
  int32_t jit_level                     = 0;

  int32_t inter_op_parallelism_threads  = 32;
  int32_t intra_op_parallelism_threads  = 1;

  std::string detail() noexcept {
    return "name: " + name + ", version: " + version + ", graph_file_loc: " + graph_file_loc
      + ", input_nodes: " + std::to_string(input_nodes.size())
      + ", output_nodes: " + std::to_string(output_nodes.size())
      + ", opt_level: " + std::to_string(opt_level) + ", jit_level: " + std::to_string(jit_level)
      + ", inter_op_parallelism_threads: " + std::to_string(inter_op_parallelism_threads)
      + ", intra_op_parallelism_threads: " + std::to_string(intra_op_parallelism_threads);
  }

  std::string brief() noexcept {
    return name + ":" + version;
  }
};

class Engine {
 public:
  explicit Engine(const EngineConf& engine_conf) noexcept(false) : conf_(engine_conf) {}
  virtual ~Engine() {}

  Engine() = delete;
  Engine& operator=(const Engine&) = delete;
  Engine(const Engine&) = delete;

  // Get brand of engine
  virtual std::string brand() noexcept = 0;

  // Perform inference
  virtual void infer(Instance *instance, Score *score) noexcept(false) = 0;

  // Perform inference with trace
  virtual void trace(Instance *instance, Score *score) noexcept(false) = 0;

  // Get input name and shape
  virtual void get_input_name_and_shape(
    absl::flat_hash_map<std::string, std::vector<int64_t>> *input_shapes
  ) noexcept(false) = 0;  // NOLINT

  // Get output name and shape
  virtual void get_output_name_and_shape(
    absl::flat_hash_map<std::string, std::vector<int64_t>> *output_shapes
  ) noexcept(false) = 0;  // NOLINT

  std::vector<Sample> *random_sample_gen(int32_t sample_count, int32_t batch_size) noexcept(false) {
    std::vector<Sample> samples;
    absl::flat_hash_map<std::string, std::vector<int64_t>> input_shapes;
    get_input_name_and_shape(&input_shapes);
    absl::flat_hash_map<std::string, std::vector<int64_t>> output_shapes;
    get_output_name_and_shape(&output_shapes);

    samples.resize(sample_count);
    std::random_device rd;
    std::mt19937 gen(rd());
    BS::thread_pool works(16);
    for (auto& sample : samples) {
      works.push_task([&](){
        sample.instance.features.resize(input_shapes.size());
        int32_t i = 0;
        for (const auto& input : input_shapes) {
          auto& feature = sample.instance.features[i++];
          feature.batch_size = batch_size;
          int64_t data_size = batch_size;
          for (auto& dim : input.second) {
            if (dim > 0) {
              data_size *= dim;
            }
          }
          feature.name = input.first;
          feature.data.resize(data_size);

          // fill random data
          for (auto& data : feature.data) {
            data = std::uniform_real_distribution<float>(0.0, 1.0)(gen);
          }
        }

        sample.score.targets.resize(output_shapes.size());
        int32_t j = 0;
        for (const auto& output : output_shapes) {
          auto& target = sample.score.targets[j++];
          target.name = output.first;
          target.batch_size = batch_size;
        }
      });
    }
    works.wait_for_tasks();
  }
);

 protected:
  // Initialize engine
  void init() {
    load();
    build();
    set_session_options();
    create_session();
    sub_init();
  }

  // Load the TensorFlow graph from the .pb file
  virtual void load() = 0;

  // Build engine
  virtual void build() = 0;

  // Set session options
  virtual void set_session_options() = 0;

  // Create session
  virtual void create_session() = 0;

  // Personalized initialization for subclasses
  virtual void sub_init() {}

  EngineConf conf_;
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_ENGINE_ENGINE_H_
