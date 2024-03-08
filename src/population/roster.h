// Copyright (C) 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_POPULATION_ROSTER_H_
#define MODEL_SERVER_SRC_POPULATION_ROSTER_H_

#include <vector>
#include <string>
#include "absl/container/flat_hash_map.h"

namespace model_server {

struct IndivadualInfo {
  std::string name;
  std::string age;
  std::string home_path;

  std::string graph_file_loc() const noexcept(false);
  std::string model_conf_loc() const noexcept(false);
};

struct Roster {
  absl::flat_hash_map<std::string, IndivadualInfo> indivaduals;
  void load(const std::string& path) noexcept(false);
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_POPULATION_ROSTER_H_
