// Copyright 2023 zh.luxu1986@gmail.com

#include "model_server/src/population/lifecycle.h"

namespace model_server {

Lifecycle::Lifecycle(const IndivadualInfo& indivadual_info) noexcept(false) :
  engine_() {}
Lifecycle::~Lifecycle() = default;

void Lifecycle::age(const std::string& new_age) noexcept(false) {}
void Lifecycle::undertake(Instance *instance, Score *score) noexcept(false) {
  engine_->infer(instance, score);
}

}  // namespace model_server
