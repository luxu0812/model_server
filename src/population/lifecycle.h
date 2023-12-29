// Copyright (C) 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_POPULATION_LIFECYCLE_H_
#define MODEL_SERVER_SRC_POPULATION_LIFECYCLE_H_

#include <memory>
#include <vector>
#include <string>
#include "model_server/src/engine/sample.h"
#include "model_server/src/engine/engine.h"
#include "model_server/src/embedding/embedding.h"
#include "model_server/src/population/roster.h"
#include "model_server/src/population/model_spec.h"

namespace model_server {

class Lifecycle {
 public:
  explicit Lifecycle(const IndivadualInfo& indivadual_info) noexcept(false);
  virtual ~Lifecycle();

  Lifecycle& operator=(const Lifecycle&) = delete;
  Lifecycle(const Lifecycle&) = delete;

  void age(const std::string& new_age) noexcept(false);
  void undertake(Instance *instance, Score *score) noexcept(false);

 private:
  std::vector<std::string>   memories_;
  std::string                age_;
  IndivadualInfo             indivadual_info_;
  ModelMeta                  model_meta_;
  EngineConf                 engine_conf_;
  std::unique_ptr<Engine>    engine_;
  std::unique_ptr<Embedding> embedding_;
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_POPULATION_LIFECYCLE_H_
