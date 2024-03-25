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

void TF2Engine::warmup(Instance *instance, Score *score) noexcept(false) {
  // std::shared_lock<std::shared_mutex> engine_lock(engine_mtx_);
  if (!inited_) {
    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
      + conf_.brief() + "] " + "Engine not initialized";
    throw std::runtime_error(err_msg);
  }
}

void TF2Engine::infer(Instance *instance, Score *score) noexcept(false) {
  // std::shared_lock<std::shared_mutex> engine_lock(engine_mtx_);
  if (!inited_) {
    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
      + conf_.brief() + "] " + "Engine not initialized";
    throw std::runtime_error(err_msg);
  }
}

void TF2Engine::trace(Instance *instance, Score *score) noexcept(false) {
  // std::shared_lock<std::shared_mutex> engine_lock(engine_mtx_);
  if (!inited_) {
    const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
      + conf_.brief() + "] " + "Engine not initialized";
    throw std::runtime_error(err_msg);
  }
}

void TF2Engine::load() {
}

void TF2Engine::build() {
}

void TF2Engine::set_session_options() {
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

std::unique_ptr<TF2EngineFactory> TF2EngineFactory::instance_ = nullptr;
EngineFactory *TF2EngineFactory::instance() {
  if (nullptr == instance_) {
    instance_.reset(new TF2EngineFactory());
  }
  return instance_.get();
}

}  // namespace model_server
