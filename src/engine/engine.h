// Copyright 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_ENGINE_ENGINE_H_
#define MODEL_SERVER_SRC_ENGINE_ENGINE_H_

#include <stdint.h>
#include <vector>
#include <string>
#include "model_server/src/common/sample.h"

namespace model_server {

const char kBrandTF[]       = "TensorFlow";
const char kBrandTFGPU[]    = "TensorFlow-GPU";
const char kBrandONNX[]     = "ONNX";
const char kBrandONNXDNNL[] = "ONNX-DNNL";

struct EngineConf {
  std::string name;
  std::string version;
  std::string graph_file_loc;
  std::vector<std::string> input_nodes;
  std::vector<std::string> output_nodes;

  int32_t opt_level;
  int32_t jit_level;

  int32_t inter_op_parallelism_threads;
  int32_t intra_op_parallelism_threads;

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
  explicit Engine(const EngineConf& engine_conf) : conf_(engine_conf) {}
  virtual ~Engine() {}

  Engine() = delete;
  Engine& operator=(const Engine&) = delete;
  Engine(const Engine&) = delete;

  // Get brand of engine
  virtual std::string brand() noexcept = 0;

  // Perform inference
  virtual void infer(Instance *instance, Score *score) noexcept(false) = 0;

  // Perform inference with trace
  virtual void trace() noexcept(false) = 0;

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
