// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_ENGINE_ENGINE_H_
#define INFER_ENGINE_SRC_ENGINE_ENGINE_H_

#include <string>
#include "infer_engine/src/data/model_spec.h"

namespace infer_engine {

class Engine {
 public:
  explicit Engine(const ModelSpec& model_spec) : model_spec_(model_spec) {}
  ~Engine();

  Engine() = delete;
  Engine& operator=(const Engine&) = delete;
  Engine(const Engine&) = delete;

  // perform inference
  virtual void infer() = 0;

 protected:
  // initialize the engine
  void init() {
    load_graph();
    build();
    create_session();
  }

  // destroy the engine
  void destroy() {
  }

  // Load the TensorFlow graph from the .pb file
  virtual void load_graph() = 0;

  // build engine
  virtual void build() = 0;

  // create session
  virtual void create_session() = 0;

  ModelSpec model_spec_;
};

}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_ENGINE_ENGINE_H_
