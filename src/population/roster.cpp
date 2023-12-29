// Copyright (C) 2023 zh.luxu1986@gmail.com

#include "model_server/src/population/roster.h"

namespace model_server {

std::string IndivadualInfo::graph_file_loc() const noexcept(false) {
  return home_path + "/" + name + "/" + age + "/graph";
}

std::string IndivadualInfo::model_conf_loc() const noexcept(false) {
  return home_path + "/" + name + "/model_conf.json";
}

void Roster::load(const std::string& path) noexcept(false) {}

}  // namespace model_server
