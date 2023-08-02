// Copyright 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_POPULATION_POPULATION_H_
#define MODEL_SERVER_SRC_POPULATION_POPULATION_H_

#include <string>
#include "absl/container/flat_hash_map.h"
#include "model_server/src/lifecycle/lifecycle.h"

namespace model_server {

class Population {
 public:
  Population() = default;
  virtual ~Population() = default;

  Population& operator=(const Population&) = delete;
  Population(const Population&) = delete;

  void born(const std::string& name, const std::string& home_path) noexcept(false);
  void die(const std::string& name) noexcept(false);
  *Lifecycle summon(const std::string& name) noexcept(false);

 private:
  abseil::flat_hash_map<std::string, *Lifecycle> indivaduals_;
};

}  // namespace model_server
#endif  // MODEL_SERVER_SRC_POPULATION_POPULATION_H_
