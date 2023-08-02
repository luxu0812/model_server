// Copyright 2023 zh.luxu1986@gmail.com

#include "model_server/src/population/population.h"

namespace model_server {

static const char kPopulationConfFileName[] = "__list__.json";

Population::Population(const std::string& settlement_path) noexcept :
  settlement_path_(settlement_path) {}

Population::~Population() {
  for (auto& [name, lifecycle] : indivaduals_) {
    if (lifecycle != nullptr) {
        delete lifecycle;
    }
  }
}

void Population::evolve() noexcept(false) {
  std::string pupulation_conf_file = settlement_path_ + "/" + kPopulationConfFileName;

  std::unique_lock lock(evolvement_mutex_);
  roster_.load(pupulation_conf_file);

  for (auto& [name, lifecycle] : indivaduals_) {
    auto indivadual_info = roster_.indivaduals.find(name);
    if (roster_.indivaduals.end() == indivadual_info) {
      die(indivadual_info->second);
    } else {
      lifecycle->age(roster_.indivaduals[name].age);
    }
  }

  for (const auto& [name, indivadual_info] : roster_.indivaduals) {
    if (indivaduals_.find(name) == indivaduals_.end()) {
      born(indivadual_info);
    }
  }
}

void Population::born(const IndivadualInfo& indivadual_info) noexcept(false) {
}

void Population::die(const IndivadualInfo& indivadual_info) noexcept(false) {
}

Lifecycle *Population::summon(const std::string& name) noexcept(false) {
  std::shared_lock lock(evolvement_mutex_);

  auto lifecycle = indivaduals_.find(name);
  if (indivaduals_.end() == lifecycle) {
    return nullptr;
  }

  return lifecycle->second;
}

}  // namespace model_server
