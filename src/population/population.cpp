// Copyright 2023 zh.luxu1986@gmail.com

#include "model_server/src/population/population.h"
#include "glog/logging.h"
#include "BShoshany/BS_thread_pool.hpp"

namespace model_server {

static const char kPopulationConfFileName[] = "__list__.json";
static const int32_t kEvolveThreadNum = 4;

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
    std::shared_lock lock(population_mutex_);
    indivaduals = indivaduals_;
  }

  // evolve
  {
    std::lock_guard lock(evolvement_mutex_);
    for (auto& [name, lifecycle] : indivaduals) {
      auto indivadual_info = roster->indivaduals.find(name);
      if (roster->indivaduals.end() == indivadual_info) {
        die(indivadual_info->second);
        indivaduals.erase(name);
      } else {
        lifecycle->age(roster->indivaduals[name].age);
      }
    }

    BS::thread_pool evolve_thread_pool(kEvolveThreadNum);
    for (const auto& [name, indivadual_info] : roster->indivaduals) {
      if (indivaduals.find(name) == indivaduals.end()) {
        indivaduals.try_emplace(name, nullptr);
        evolve_thread_pool.push_task(
          [this](std::shared_ptr<Lifecycle> *dest, const IndivadualInfo& indivadual_info) {
            try {
              this->born(indivadual_info, dest);
            } catch (const std::exception& e) {
              *dest = nullptr;
              LOG(ERROR) << e.what();
            } catch (...) {
              *dest = nullptr;
              LOG(ERROR) << "unknown exception";
            }
          },
          &(indivaduals[name]), indivadual_info
        ); // NOLINT
      }
    }
    evolve_thread_pool.wait_for_tasks();

    for (const auto& [name, lifecycle] : indivaduals) {
      if (nullptr == lifecycle) {
        std::string err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__)
                            + "] indivadual " + name + " born failed";
        throw std::runtime_error(err_msg);
      }
    }

    // update router_ and indivaduals_
    {
      std::unique_lock lock(population_mutex_);
      roster_.swap(roster);
      indivaduals_.swap(indivaduals);
    }
  }

  // destroy old roster and indivaduals
}

void Population::born(const IndivadualInfo& indivadual_info, std::shared_ptr<Lifecycle> *dest) noexcept(false) {
}

void Population::die(const IndivadualInfo& indivadual_info) noexcept(false) {
}

std::shared_ptr<Lifecycle> Population::summon(const std::string& name) noexcept(false) {
  std::shared_lock lock(population_mutex_);

  auto lifecycle = indivaduals_.find(name);
  if (indivaduals_.end() == lifecycle) {
    return nullptr;
  }

  return lifecycle->second;
}

}  // namespace model_server
