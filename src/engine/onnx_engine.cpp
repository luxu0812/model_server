// Copyright 2023 zh.luxu1986@gmail.com

#include <utility>
#include <vector>
#include "glog/logging.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_join.h"
#include "model_server/src/engine/onnx_engine.h"

namespace model_server {

ONNXEngine::ONNXEngine(const EngineConf& engine_conf) noexcept(false) :
  Engine(engine_conf),
  engine_mtx_(),
  inited_(false),
  env_(nullptr),
  session_opts_(nullptr),
  session_(nullptr) {
  init();
  inited_ = true;
}

ONNXEngine::~ONNXEngine() {
  try {
    std::unique_lock<std::shared_mutex> engine_lock(engine_mtx_);
    inited_ = false;

    if (nullptr != session_) {
      delete session_;
      session_ = nullptr;
      LOG(INFO) << "[" << conf_.brief() << "] Session deleted";
    }

    if (nullptr != env_) {
      delete env_;
      env_ = nullptr;
      LOG(INFO) << "[" << conf_.brief() << "] Env deleted";
    }

    if (nullptr != session_opts_) {
      delete session_opts_;
      session_opts_ = nullptr;
      LOG(INFO) << "[" << conf_.brief() << "] Session options deleted";
    }
  } catch (const std::exception& e) {
    LOG(ERROR) << e.what();
  } catch (...) {
    LOG(ERROR) << "Unknown exception";
  }
}

std::string ONNXEngine::brand() noexcept {
  return kBrandONNX;
}

  // Perform inference using the ONNX runtime
void ONNXEngine::infer(Instance *instance, Score *score) noexcept(false) {
  std::shared_lock<std::shared_mutex> engine_lock(engine_mtx_);
  if (!inited_) {
    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
      + conf_.brief() + "] " + "Engine not initialized";
    throw std::runtime_error(err_msg);
  }

  const auto& batch_size = instance->batch_size;

  // Create memory info
  Ort::MemoryInfo info = Ort::MemoryInfo::CreateCpu(
    OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault
  );  // NOLINT

  // Prepare input tensors
  std::vector<const char*> input_names;
  std::vector<Ort::Value> input_tensors;

  // Set input data to the input tensors
  for (auto& feature : instance->features) {
    const std::string& feature_name = feature.name + ":0";
    const auto it = onnx_model_meta_.input_metas.find(feature_name);
    if (onnx_model_meta_.input_metas.end() != it) {
      std::vector<int64_t> tensor_shape = it->second.shape;
      tensor_shape[0] = batch_size;
      input_tensors.push_back(Ort::Value::CreateTensor<float>(
        info, feature.data.data(), feature.data.size(), tensor_shape.data(), tensor_shape.size()
      ));  // NOLINT
      input_names.push_back(it->second.name.c_str());
    }
  }

  // Prepare output tensors
  std::vector<const char*> output_names;
  std::vector<Ort::Value> output_tensors;

  // Set output data to the output tensors
  int i = 0;
  for (const auto& [name, meta] : onnx_model_meta_.output_metas) {
    std::vector<int64_t> tensor_shape = meta.shape;
    tensor_shape[0] = batch_size;
    score->targets[i].name = name;
    score->targets[i].data.resize(meta.instance_size * batch_size);
    output_tensors.push_back(Ort::Value::CreateTensor<float>(
      info, score->targets[i].data.data(), score->targets[i].data.size(), tensor_shape.data(), tensor_shape.size()
    ));  // NOLINT
    output_names.push_back(name.c_str());
    ++i;
  }

  // Run inference using the ONNX runtime
  Ort::RunOptions run_options;
  session_->Run(
    run_options,
    input_names.data(), input_tensors.data(), input_names.size(),
    output_names.data(), output_tensors.data(), output_names.size()
  );  // NOLINT
}

void ONNXEngine::trace(Instance *instance, Score *score) noexcept(false) {
  std::shared_lock<std::shared_mutex> engine_lock(engine_mtx_);
  if (!inited_) {
    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
      + conf_.brief() + "] " + "Engine not initialized";
    throw std::runtime_error(err_msg);
  }

  // session_opts_.EnableProfiling(conf_.name.c_str());

  // static std::atomic<int> sess_cnt = 0;
  // if (sess_cnt.load() < 5) {
  //     sess_cnt.fetch_add(1);
  // } else {
  //     Ort::AllocatorWithDefaultOptions allocator;
  //     auto profile_file = _session->EndProfilingAllocated(allocator);
  //     if (std::string(profile_file.get()) != std::string()) {
  //         LOG(INFO) << "ONNX profiling file has dump to " << std::string(profile_file.get());
  //     }
  // }
}

void ONNXEngine::load() {
}

void ONNXEngine::build() {
  // build engine
}

void ONNXEngine::set_session_options() {
  // set session options

  session_opts_ = new Ort::SessionOptions();
  session_opts_->SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
  session_opts_->SetIntraOpNumThreads(conf_.intra_op_parallelism_threads);
  session_opts_->SetInterOpNumThreads(conf_.inter_op_parallelism_threads);
  // session_opts_->DisablePerSessionThreads();

  LOG(INFO) << "[" << conf_.brief() << "] Session options set";
}

void ONNXEngine::create_session() {
  // create session
  env_ = new Ort::Env(OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING, conf_.name.c_str());
  session_ = new Ort::Session(*env_, conf_.graph_file_loc.c_str(), *session_opts_);
  LOG(INFO) << "[" << conf_.brief() << "] Session created";
}

void ONNXEngine::sub_init() {
  Ort::AllocatorWithDefaultOptions allocator;

  // Get input nodes info
  size_t num_input_nodes = session_->GetInputCount();
  for (int32_t i = 0; i < static_cast<int32_t>(num_input_nodes); ++i) {
    std::string input_name = std::string(session_->GetInputNameAllocated(i, allocator).get());
    auto tensor_shape = session_->GetInputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape();

    size_t instance_size = 1;
    std::vector<int64_t> shape;
    for (const auto& dim : tensor_shape) {
      shape.push_back(dim);
      if (dim > 0) {
        instance_size *= dim;
      }
    }

    onnx_model_meta_.input_metas[input_name] = ONNXTensorMeta{
      .name          = input_name,
      .num_dims      = static_cast<int32_t>(tensor_shape.size()),
      .shape         = shape,
      .instance_size = instance_size,
      .index         = i
    };
  }

  // Get output nodes info
  size_t num_output_nodes = session_->GetOutputCount();
  for (int32_t i = 0; i < static_cast<int32_t>(num_output_nodes); ++i) {
    std::string output_name = std::string(session_->GetOutputNameAllocated(i, allocator).get());
    auto tensor_shape = session_->GetOutputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape();

    size_t instance_size = 1;
    std::vector<int64_t> shape;
    for (const auto& dim : tensor_shape) {
      shape.push_back(dim);
      if (dim > 0) {
        instance_size *= dim;
      }
    }

    onnx_model_meta_.output_metas[output_name] = ONNXTensorMeta{
      .name          = output_name,
      .num_dims      = static_cast<int32_t>(tensor_shape.size()),
      .shape         = shape,
      .instance_size = instance_size,
      .index         = i
    };
  }

  LOG(INFO) << onnx_model_meta_.to_string();
}

std::string ONNXTensorMeta::to_string() {
  std::string message;
  absl::StrAppendFormat(&message,
    "  num_dims: %d\n  instance_size: %llu\n  index: %d\n  shape: %s",
    num_dims, instance_size, index, absl::StrJoin(shape, ", ").c_str()
  );  // NOLINT

  return message;
}

std::string ONNXModelMeta::to_string() {
  std::string message;
  absl::StrAppendFormat(&message, "\ninput_metas: ");
  for (auto& entry : input_metas) {
    absl::StrAppendFormat(&message, "\n %s:", entry.first.c_str());
    absl::StrAppendFormat(&message, "\n%s", entry.second.to_string().c_str());
  }

  absl::StrAppendFormat(&message, "\noutput_metas:");
  for (auto& entry : output_metas) {
    absl::StrAppendFormat(&message, "\n %s:", entry.first.c_str());
    absl::StrAppendFormat(&message, "\n%s", entry.second.to_string().c_str());
  }

  return message;
}

}  // namespace model_server
