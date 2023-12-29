// Copyright 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_ENGINE_TF_ENGINE_H_
#define MODEL_SERVER_SRC_ENGINE_TF_ENGINE_H_

#include <stdint.h>
#include <vector>
#include <string>
#include <functional>
#include <shared_mutex>
#include "absl/container/flat_hash_map.h"
#include "tensorflow/c/c_api.h"
#include "model_server/src/engine/engine.h"

namespace model_server {

const char kGlobalInterOpThreadPool[] = "GlobalTensorFlowInterOpThreadPool";

struct TFTensorMeta {
  TF_Output           *output = nullptr;
  std::string          operation_name;
  std::string          operation_type;
  int32_t              operation_num_inputs;
  int32_t              operation_num_outputs;
  TF_DataType          data_type;
  int32_t              data_size;
  int32_t              num_dims;
  std::vector<int64_t> shape;
  size_t               instance_size;
  int32_t              index;

  std::string to_string();
};

struct TFModelMeta {
  absl::flat_hash_map<std::string, TFTensorMeta> input_metas;
  absl::flat_hash_map<std::string, TFTensorMeta> output_metas;
  std::vector<TF_Output> input_specs;
  std::vector<TF_Output> output_specs;

  std::string to_string();
};

class TFEngine : public Engine {
 public:
  explicit TFEngine(const EngineConf& engine_conf) noexcept(false);
  virtual ~TFEngine();

  TFEngine() = delete;
  TFEngine& operator=(const TFEngine&) = delete;
  TFEngine(const TFEngine&) = delete;

  // Get brand of engine
  std::string brand() noexcept override;

  // Perform inference using the TF runtime
  void infer(Instance *instance, Score *score) noexcept(false) override;

  // Perform inference with trace using the TF runtime
  void trace(Instance *instance, Score *score) noexcept(false) override;

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
  void run_session(
    std::vector<TF_Tensor*> *input_tensors, std::vector<TF_Tensor*> *output_tensors,
    TF_Buffer *tf_run_opts = nullptr, TF_Buffer *tf_metadata = nullptr
  );  // NOLINT

  // Iterate through the operations in the graph
  void iterate_through_operations(std::function<void(TF_Operation*)> do_something_with_operation);

  // Get TFTensorMeta by TF_Operation name
  void get_tf_tensor_meta_by_tf_operation_name(
    const std::string& tf_operation_name,
    absl::flat_hash_map<std::string, TFTensorMeta> *tf_tensor_meta,
    int32_t *index = nullptr
  );  // NOLINT

  // Convert TF_Output to TFTensorMeta
  void convert_tf_output_to_tf_tensor_meta(
    const TF_Output& tf_output, TFTensorMeta *tf_tensor_meta
  );  // NOLINT

  // Get input and output ops
  // void get_input_output_ops();

  // Print graph information
  // void print_graph_info();

  void instance_to_tensor(
    Instance *instance, std::vector<TF_Tensor*> *input_tensors
  ) noexcept(false);  // NOLINT
  void score_from_tensor(
    const std::vector<TF_Tensor*>& output_tensors, Score *score
  ) noexcept(false);  // NOLINT

 private:
  // Preventing from distructing during inference, should be gurranteed by caller
  // std::shared_mutex engine_mtx_;
  bool inited_;

  TFModelMeta tf_model_meta_;

  TF_Buffer         *graph_buffer_;
  TF_Graph          *graph_;
  TF_SessionOptions *session_opts_;
  TF_Session        *session_;
  TF_Buffer         *_default_run_option_buf;
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_ENGINE_TF_ENGINE_H_
