// Copyright 2023 zh.luxu1986@gmail.com

#include "model_server/src/population/lifecycle.h"
#include <string>

namespace model_server {

Lifecycle::Lifecycle(const IndivadualInfo& indivadual_info) noexcept(false) :
  indivadual_info_(indivadual_info) {
  model_meta_.load(indivadual_info_.model_conf_path());
}

Lifecycle::~Lifecycle() = default;

void Lifecycle::age(const std::string& new_age) noexcept(false) {}
void Lifecycle::undertake(Instance *instance, Score *score) noexcept(false) {
  engine_->infer(instance, score);
}

}  // namespace model_server
