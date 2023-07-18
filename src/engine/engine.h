// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_ENGINE_ENGINE_H_
#define INFER_ENGINE_SRC_ENGINE_ENGINE_H_

#include <stdint.h>
#include <string>
#include "infer_engine/src/data/model_spec.h"

namespace infer_engine {

const char kBrandTF[]       = "TensorFlow";
const char kBrandTFGPU[]    = "TensorFlow-GPU";
const char kBrandONNX[]     = "ONNX";
const char kBrandONNXDNNL[] = "ONNX-DNNL";

class Engine {
 public:
  explicit Engine(const ModelSpec& model_spec) : model_spec_(model_spec) {}
  ~Engine() {}

  Engine() = delete;
  Engine& operator=(const Engine&) = delete;
  Engine(const Engine&) = delete;

  // Get brand of engine
  virtual std::string brand() = 0;

  // Perform inference
  virtual void infer(const int32_t batch_size, const void *input, void *output) = 0;

  // Perform inference with trace
  virtual void trace() = 0;

 protected:
  // Initialize engine
  void init() {
    load_model_meta();
    load();
    build();
    set_session_options();
    create_session();
  }

  // Load model meta data
  void load_model_meta() {
    model_meta_.load(model_spec_.meta_file);
  }

  // Load the TensorFlow graph from the .pb file
  virtual void load() = 0;

  // Build engine
  virtual void build() = 0;

  // Set session options
  virtual void set_session_options() = 0;

  // Create session
  virtual void create_session() = 0;

  ModelSpec model_spec_;
  ModelMeta model_meta_;
};

}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_ENGINE_ENGINE_H_
