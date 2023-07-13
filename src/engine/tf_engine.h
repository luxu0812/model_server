// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_ENGINE_TF_ENGINE_H_
#define INFER_ENGINE_SRC_ENGINE_TF_ENGINE_H_

#include <string>
#include "tensorflow/c/c_api.h"
#include "infer_engine/src/engine/engine.h"

namespace infer_engine {

class TFEngine : public Engine {
 public:
  explicit TFEngine(const ModelSpec& model_spec);
  ~TFEngine();

  TFEngine() = delete;
  TFEngine& operator=(const TFEngine&) = delete;
  TFEngine(const TFEngine&) = delete;

  // perform inference
  void infer() override;

 protected:
  // read graph from file
  void read_graph(const std::string &graph_file) override;

 private:
  TF_Session *session_;
  TF_Graph   *graph_;
  TF_Buffer  *graph_buffer_;
};

}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_ENGINE_TF_ENGINE_H_
