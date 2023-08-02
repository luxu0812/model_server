// Copyright 2023 zh.luxu1986@gmail.com

#include "model_server/src/population/population.h"

namespace model_server {

static const char kPopulationConfFileName[] = "__list__.json";

Population::Population(const std::string& settlement_path) noexcept :
  settlement_path_(settlement_path) {}

Population::~Population() {}

void Population::evolve() noexcept(false) {
  std::string pupulation_conf_file = settlement_path_ + "/" + kPopulationConfFileName;

  // create new roster and indivaduals for temporary use
  std::unique_ptr<Roster> roster(new Roster());
  roster->load(pupulation_conf_file);
  absl::flat_hash_map<std::string, std::shared_ptr<Lifecycle>> indivaduals;
  {
    std::shared_lock lock(evolvement_mutex_);
    indivaduals = indivaduals_;
  }

  // evolve
  for (auto& [name, lifecycle] : indivaduals) {
    auto indivadual_info = roster->indivaduals.find(name);
    if (roster->indivaduals.end() == indivadual_info) {
      die(indivadual_info->second);
    } else {
      lifecycle->age(roster->indivaduals[name].age);
    }
  }
  for (const auto& [name, indivadual_info] : roster->indivaduals) {
    if (indivaduals.find(name) == indivaduals.end()) {
      born(indivadual_info);
    }
  }

  // update router_ and indivaduals_
  {
    std::unique_lock lock(evolvement_mutex_);
    roster_.swap(roster);
    indivaduals_.swap(indivaduals);
  }

  // destroy old roster and indivaduals
}

void Population::born(const IndivadualInfo& indivadual_info) noexcept(false) {
}

void Population::die(const IndivadualInfo& indivadual_info) noexcept(false) {
}

std::shared_ptr<Lifecycle> Population::summon(const std::string& name) noexcept(false) {
  std::shared_lock lock(evolvement_mutex_);

  auto lifecycle = indivaduals_.find(name);
  if (indivaduals_.end() == lifecycle) {
    return nullptr;
  }

  return lifecycle->second;
}

}  // namespace model_server
