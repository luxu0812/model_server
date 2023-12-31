// Copyright (C) 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_ENGINE_ONNX_ENGINE_H_
#define MODEL_SERVER_SRC_ENGINE_ONNX_ENGINE_H_

#include <vector>
#include <string>
#include <shared_mutex>
#include "absl/container/flat_hash_map.h"
#include "onnxruntime/core/session/onnxruntime_cxx_api.h"
#include "model_server/src/engine/engine.h"

namespace model_server {

struct ONNXTensorMeta {
  std::string          name;
  int32_t              num_dims;
  std::vector<int64_t> shape;
  size_t               instance_size;
  int32_t              index;

  std::string to_string();
};

struct ONNXModelMeta {
  absl::flat_hash_map<std::string, ONNXTensorMeta> input_metas;
  absl::flat_hash_map<std::string, ONNXTensorMeta> output_metas;

  std::string to_string();
};

class ONNXEngine : public Engine {
 public:
  explicit ONNXEngine(const EngineConf& engine_conf) noexcept(false);
  virtual ~ONNXEngine();

  ONNXEngine() = delete;
  ONNXEngine& operator=(const ONNXEngine&) = delete;
  ONNXEngine(const ONNXEngine&) = delete;

  // Get brand of engine
  std::string brand() noexcept override;

  // Perform inference using the ONNX runtime
  void infer(Instance *instance, Score *score) noexcept(false) override;

  // Perform inference with trace using the ONNX runtime
  void trace(Instance *instance, Score *score) noexcept(false) override;

  // Get input name and shape
  void get_input_name_and_shape(
    absl::flat_hash_map<std::string, std::vector<int64_t>> *input_shapes
  ) noexcept(false) override;  // NOLINT

  // Get output name and shape
  void get_output_name_and_shape(
    absl::flat_hash_map<std::string, std::vector<int64_t>> *output_shapes
  ) noexcept(false) override;  // NOLINT

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

  void run_session(Instance *instance, Score *score, Ort::Session *session) noexcept(false);

 protected:
  Ort::SessionOptions *session_opts_;

 private:
  // Preventing from distructing during inference, should be gurranteed by caller
  // std::shared_mutex engine_mtx_;
  bool inited_;

  Ort::Env            *env_;
  Ort::Session        *session_;

  ONNXModelMeta onnx_model_meta_;
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_ENGINE_ONNX_ENGINE_H_
