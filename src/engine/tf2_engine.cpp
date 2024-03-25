// Copyright (C) 2023 zh.luxu1986@gmail.com

#include "model_server/src/engine/tf2_engine.h"

#include <fstream>
#include <string>

#include "absl/log/log.h"
#include "absl/cleanup/cleanup.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_join.h"
#include "tensorflow/c/c_api.h"
#include "tensorflow/core/protobuf/config.pb.h"

namespace model_server {

TF2Engine::TF2Engine(const EngineConf& engine_conf) noexcept(false) :
  Engine(engine_conf),
  // engine_mtx_(),
  tags_(),
  session_opts_(),
  run_opts_(),
  bundle_() {
}

TF2Engine::~TF2Engine() {
  try {
    // std::unique_lock<std::shared_mutex> engine_lock(engine_mtx_);
    inited_ = false;
  } catch (const std::exception& e) {
    LOG(ERROR) << e.what();
  } catch (...) {
    LOG(ERROR) << "Unknown exception";
  }
}

std::string TF2Engine::brand() noexcept {
  return kBrandTF2;
}

void TF2Engine::instance_to_tensor(
  Instance *instance, std::vector<TF_Tensor*> *input_tensors
) {
}

void TF2Engine::score_from_tensor(
  const std::vector<TF_Tensor*>& output_tensors, Score *score
) {
}

void TF2Engine::warmup(Instance *instance, Score *score) noexcept(false) {
  // std::shared_lock<std::shared_mutex> engine_lock(engine_mtx_);
  if (!inited_) {
    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
      + conf_.brief() + "] " + "Engine not initialized";
    throw std::runtime_error(err_msg);
  }

  instance_to_tensor(instance, &input_tensors);
  run_session(&input_tensors, &output_tensors, warmup_run_option_buf_);
  score_from_tensor(output_tensors, score);
}

void TF2Engine::infer(Instance *instance, Score *score) noexcept(false) {
  // std::shared_lock<std::shared_mutex> engine_lock(engine_mtx_);
  if (!inited_) {
    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
      + conf_.brief() + "] " + "Engine not initialized";
    throw std::runtime_error(err_msg);
  }

  instance_to_tensor(instance, &input_tensors);
  run_session(&input_tensors, &output_tensors, default_run_option_buf_);
  score_from_tensor(output_tensors, score);
}

void TF2Engine::trace(Instance *instance, Score *score) noexcept(false) {
  // std::shared_lock<std::shared_mutex> engine_lock(engine_mtx_);
  if (!inited_) {
    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
      + conf_.brief() + "] " + "Engine not initialized";
    throw std::runtime_error(err_msg);
  }

  instance_to_tensor(instance, &input_tensors);
  run_session(&input_tensors, &output_tensors, tf_run_opts_data, tf_metadata);
  score_from_tensor(output_tensors, score);
}

void TF2Engine::run_session(
  std::vector<TF_Tensor*> *input_tensors,
  std::vector<TF_Tensor*> *output_tensors,
  TF_Buffer *tf_run_opts,
  TF_Buffer *tf_metadata
) {
}

void TF2Engine::load() {
}

void TF2Engine::build() {
}

void TF2Engine::set_session_options() {
  // refer to: https://github.com/tensorflow/tensorflow/blob/master/tensorflow/core/protobuf/config.proto
  TF_Status *tf_status = TF_NewStatus();
  auto tf_status_cleanup = absl::MakeCleanup([&tf_status]() { TF_DeleteStatus(tf_status); });

  tensorflow::OptimizerOptions tf_optimizer_opts;
  tf_optimizer_opts.set_do_constant_folding(true);
  tf_optimizer_opts.set_do_function_inlining(true);
  if (0 == conf_.opt_level) {
    tf_optimizer_opts.set_opt_level(tensorflow::OptimizerOptions_Level_L0);
  } else {
    tf_optimizer_opts.set_opt_level(tensorflow::OptimizerOptions_Level_L1);
  }
  if (0 == conf_.jit_level) {
    tf_optimizer_opts.set_cpu_global_jit(false);
    tf_optimizer_opts.set_global_jit_level(tensorflow::OptimizerOptions_GlobalJitLevel_OFF);
  } else if (1 == conf_.jit_level) {
    tf_optimizer_opts.set_cpu_global_jit(true);
    tf_optimizer_opts.set_global_jit_level(tensorflow::OptimizerOptions_GlobalJitLevel_ON_1);
  } else {
    tf_optimizer_opts.set_cpu_global_jit(true);
    tf_optimizer_opts.set_global_jit_level(tensorflow::OptimizerOptions_GlobalJitLevel_ON_2);
  }

  tensorflow::ConfigProto tf_session_conf;
  tf_session_conf.mutable_graph_options()->mutable_optimizer_options()->CopyFrom(tf_optimizer_opts);
  tf_session_conf.set_intra_op_parallelism_threads(conf_.intra_op_parallelism_threads);
  tf_session_conf.set_inter_op_parallelism_threads(conf_.inter_op_parallelism_threads);
  set_gpu(&tf_session_conf);

  {
    tensorflow::ThreadPoolOptionProto *thread_pool_opt =
      tf_session_conf.mutable_session_inter_op_thread_pool()->Add();
    thread_pool_opt->set_num_threads(1);
    thread_pool_opt->set_global_name(std::string(kGlobalInterOpThreadPool) + "_warmup");
  }

  if (conf_.use_global_thread_pool) {
    tf_session_conf.set_use_per_session_threads(false);
    tensorflow::ThreadPoolOptionProto *thread_pool_opt =
      tf_session_conf.mutable_session_inter_op_thread_pool()->Add();
    thread_pool_opt->set_num_threads(conf_.inter_op_parallelism_threads);
    thread_pool_opt->set_global_name(
      std::string(kGlobalInterOpThreadPool) + "_"
      + std::to_string(conf_.inter_op_parallelism_threads)
    );  // NOLINT
  } else {
    tf_session_conf.set_use_per_session_threads(true);
  }

  // tf_session_conf.mutable_experimental()->set_collective_deterministic_sequential_execution(true);
  // tf_session_conf.mutable_experimental()->set_collective_nccl(true);
  // tf_session_conf.mutable_experimental()->set_disable_thread_spinning(true);
  // tf_session_conf.mutable_experimental()->set_optimize_for_static_graph(true);
  // tf_session_conf.mutable_experimental()->set_use_tfrt(true);
  // tf_session_conf.mutable_experimental()->set_xla_fusion_autotuner_thresh(256);
  LOG(INFO) << "[" << conf_.detail() << "] Session config:\n" << tf_session_conf.DebugString();

  std::string tf_session_conf_str;
  tf_session_conf.SerializeToString(&tf_session_conf_str);

  session_opts_ = TF_NewSessionOptions();
  TF_SetConfig(session_opts_, tf_session_conf_str.data(), tf_session_conf_str.size(), tf_status);
  if (TF_GetCode(tf_status) != TF_OK) {
    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
      + conf_.brief() + "] " + "Failed to set session config: " + std::string(TF_Message(tf_status));
    throw std::runtime_error(err_msg);
  }

  tensorflow::RunOptions default_run_opt;
  default_run_opt.mutable_experimental()->set_use_run_handler_pool(true);
  default_run_opt.mutable_experimental()->mutable_run_handler_pool_options()->set_priority(1);
  if (conf_.use_global_thread_pool) {
    default_run_opt.set_inter_op_thread_pool(1);
  }
  std::string default_run_opt_str;
  default_run_opt.SerializeToString(&default_run_opt_str);
  default_run_option_buf_ = TF_NewBufferFromString(
    static_cast<void*>(default_run_opt_str.data()), default_run_opt_str.size()
  );  // NOLINT

  LOG(INFO) << "[" << conf_.brief() << "] Session options set";

  default_run_opt.set_inter_op_thread_pool(0);
  default_run_opt.SerializeToString(&default_run_opt_str);
  default_run_opt.mutable_experimental()->set_use_run_handler_pool(false);
  warmup_run_option_buf_ = TF_NewBufferFromString(
    static_cast<void*>(default_run_opt_str.data()), default_run_opt_str.size()
  );  // NOLINT

  tags_.add(tensorflow::kSavedModelTagServe);
}

void TF2Engine::set_gpu(tensorflow::ConfigProto *tf_session_conf) noexcept(false) {
  if (nullptr == tf_session_conf) {
    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "] "
      + "Session config is nullptr";
    throw std::runtime_error(err_msg);
  }

  (*(tf_session_conf->mutable_device_count()))["GPU"] = 0;
  // (*(tf_session_conf->mutable_device_count()))["CPU"] = 1;

  tags.add(tensorflow::kSavedModelTagGpu);
}

void TF2Engine::create_session() {
  Status status = tensorflow::LoadSavedModel(
    session_options_, run_options_, conf_.graph_file_loc, tags_, &model_bundle_
  );  // NOLINT
}

void TF2Engine::sub_init() {
}

void TF2Engine::get_input_name_and_shape(
  absl::flat_hash_map<std::string, std::vector<int64_t>> *input_shapes
) {
  auto graph_def_ = bundle_.meta_graph_def.graph_def();
  for (const auto& node : graph_def.node()) {
    if (node.op() == "Placeholder") {
        const auto& shape_attr = node.attr().at("shape");
        (*input_shapes)[node.name()].resize(shape_attr.shape().dim_size());
        for (int i = 0; i < shape_attr.shape().dim_size(); ++i) {
          (*input_shapes)[node.name()][i] = shape_attr.shape().dim(i).size();
        }
    }
  }
}

void TF2Engine::get_output_name_and_shape(
  absl::flat_hash_map<std::string, std::vector<int64_t>> *output_shapes
) {
}

std::unique_ptr<TF2EngineFactory> TF2EngineFactory::instance_ = nullptr;
EngineFactory *TF2EngineFactory::instance() {
  if (nullptr == instance_) {
    instance_.reset(new TF2EngineFactory());
  }
  return instance_.get();
}

}  // namespace model_server
