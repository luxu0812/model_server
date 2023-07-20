// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_ENGINE_TF_ENGINE_H_
#define INFER_ENGINE_SRC_ENGINE_TF_ENGINE_H_

#include <stdint.h>
#include <vector>
#include <string>
#include "tensorflow/c/c_api.h"
#include "infer_engine/src/engine/engine.h"

namespace infer_engine {

struct TFModelMeta {
  std::vector<TF_Operation *> input_ops;
  std::vector<TF_Operation *> output_ops;
};

class TFEngine : public Engine {
 public:
  explicit TFEngine(const ModelSpec& model_spec);
  ~TFEngine();

  TFEngine() = delete;
  TFEngine& operator=(const TFEngine&) = delete;
  TFEngine(const TFEngine&) = delete;

  // Get brand of engine
  std::string brand() override;

  // Perform inference using the TF runtime
  void infer(const int32_t batch_size, const void *input, void *output) override;
  void infer(const BatchInstance& batch_instance, BatchScore *batch_score) override;

  // Perform inference with trace using the TF runtime
  void trace() override;

 protected:
  // Load the TensorFlow graph from the .pb file
  void load() override;

  // Build engine
  void build() override;

  // Set session options
  void set_session_options() override;

  // Create session
  void create_session() override;

  // Sub initialization
  void sub_init() override;

  // Run session
  void run_session(TF_Buffer *tf_run_opts = nullptr, TF_Buffer *tf_metadata = nullptr);

  // Get input and output ops
  // void get_input_output_ops();

  // Print graph information
  // void print_graph_info();

 private:
  TFModelMeta tf_model_meta_;

  TF_Buffer         *graph_buffer_;
  TF_Graph          *graph_;
  TF_SessionOptions *session_opts_;
  TF_Session        *session_;
};

}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_ENGINE_TF_ENGINE_H_
