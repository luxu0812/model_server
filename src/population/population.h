// Copyright 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_POPULATION_POPULATION_H_
#define MODEL_SERVER_SRC_POPULATION_POPULATION_H_

#include <mutex>  // NOLINT
#include <string>
#include "absl/container/flat_hash_map.h"
#include "model_server/src/data/roster.h"
#include "model_server/src/lifecycle/lifecycle.h"

namespace model_server {

class Population {
 public:
  explicit Population(std::string settlement_path) noexcept;
  ~Population();

  Population& operator=(const Population&) = delete;
  Population(const Population&) = delete;

  void evolve() noexcept(false);
  void born(const IndivadualInfo& indivadual) noexcept(false);
  void die(const IndivadualInfo& indivadual) noexcept(false);
  *Lifecycle summon(const std::string& name) noexcept(false);

 private:
  std::mutex evolvement_mutex_;
  std::string settlement_path_;
  Roster roster_;

  abseil::flat_hash_map<std::string, *Lifecycle> indivaduals_;
};

}  // namespace model_server
#endif  // MODEL_SERVER_SRC_POPULATION_POPULATION_H_
