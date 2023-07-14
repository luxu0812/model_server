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

  // Perform inference using the TF runtime
  void infer() override;

  // Perform inference with trace using the TF runtime
  void trace() override;

 protected:
  // Load the TensorFlow graph from the .pb file
  void load_graph() override;

  // Build engine
  void build() override;

  // Create session
  void create_session() override;

  // Run session
  void run_session(TF_Buffer *tf_run_opts = nullptr, TF_Buffer *tf_metadata = nullptr);

 private:
  TF_Session *session_;
  TF_Graph   *graph_;
  TF_Buffer  *graph_buffer_;
};

}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_ENGINE_TF_ENGINE_H_
