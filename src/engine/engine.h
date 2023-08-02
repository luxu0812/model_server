// Copyright 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_ENGINE_ENGINE_H_
#define MODEL_SERVER_SRC_ENGINE_ENGINE_H_

#include <stdint.h>
#include <string>
#include "model_server/src/data/sample.h"
#include "model_server/src/data/model_spec.h"
#include "model_server/src/data/runtime_conf.h"

namespace model_server {

const char kBrandTF[]       = "TensorFlow";
const char kBrandTFGPU[]    = "TensorFlow-GPU";
const char kBrandONNX[]     = "ONNX";
const char kBrandONNXDNNL[] = "ONNX-DNNL";

class Engine {
 public:
  Engine(const ModelSpec& model_spec, const RuntimeConf& runtime_conf) :
    model_spec_(model_spec),
    runtime_conf_(runtime_conf) {}
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
    load_model_meta();
    load();
    build();
    set_session_options();
    create_session();
    sub_init();
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

  // Personalized initialization for subclasses
  virtual void sub_init() {}

  ModelSpec model_spec_;
  ModelMeta model_meta_;

  RuntimeConf runtime_conf_;
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_ENGINE_ENGINE_H_
