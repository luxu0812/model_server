// Copyright 2023 zh.luxu1986@gmail.com

#include "model_server/src/population/population.h"

namespace model_server {

static const char kPopulationConfFileName[] = "__list__.json";

Population::Population(const string& settlement_path) noexcept :
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

  std::lock_guard<std::mutex> lock(evolvement_mutex_);
  roster_.load(pupulation_conf_file);

  for (auto& indivadual : roster_.indivaduals) {
    if (indivaduals_.find(indivadual.name) != indivaduals_.end()) {
      indivaduals_[indivadual.name]->age(indivadual.age);
    } else {
      born(indivadual);
    }
  }
}

void Population::born(const IndivadualInfo& indivadual) noexcept(false) {
}

void Population::die(const IndivadualInfo& indivadual) noexcept(false) {
}

*Lifecycle Population::summon(const std::string& name) noexcept(false) {
}

}  // namespace model_server
