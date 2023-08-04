// Copyright 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_POPULATION_LIFECYCLE_H_
#define MODEL_SERVER_SRC_POPULATION_LIFECYCLE_H_

#include <vector>
#include <string>
#include "model_server/src/common/sample.h"
#include "model_server/src/engine/engine.h"
#include "model_server/src/population/roster.h"

namespace model_server {

class Lifecycle {
 public:
  explicit Lifecycle(const IndivadualInfo& indivadual_info) {}
  virtual ~Lifecycle() = default;

  Lifecycle& operator=(const Lifecycle&) = delete;
  Lifecycle(const Lifecycle&) = delete;

  void age(const std::string& new_age) {}
  void undertake(Instance *instance, Score *score) {}

 private:
  std::vector<std::string> memories_;
  std::string              age_;
  Engine*                  engine_;
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_POPULATION_LIFECYCLE_H_
