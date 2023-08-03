// Copyright 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_POPULATION_POPULATION_H_
#define MODEL_SERVER_SRC_POPULATION_POPULATION_H_

#include <memory>
#include <mutex>  // NOLINT
#include <shared_mutex>
#include <string>
#include "absl/container/flat_hash_map.h"
#include "model_server/src/data/roster.h"
#include "model_server/src/lifecycle/lifecycle.h"

namespace model_server {

class Population {
 public:
  explicit Population(const std::string& settlement_path) noexcept;
  ~Population();

  Population& operator=(const Population&) = delete;
  Population(const Population&) = delete;

  void evolve() noexcept(false);
  std::shared_ptr<Lifecycle> summon(const std::string& name) noexcept(false);

 private:
  void born(const IndivadualInfo& indivadual_info, std::shared_ptr<Lifecycle> *dest) noexcept(false);
  void die(const IndivadualInfo& indivadual_info) noexcept(false);

  std::mutex evolvement_mutex_;
  std::shared_mutex population_mutex_;
  std::string settlement_path_;
  std::unique_ptr<Roster> roster_;
  absl::flat_hash_map<std::string, std::shared_ptr<Lifecycle>> indivaduals_;
};

}  // namespace model_server
#endif  // MODEL_SERVER_SRC_POPULATION_POPULATION_H_
