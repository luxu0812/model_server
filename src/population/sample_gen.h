// Copyright (C) 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_POPULATION_SAMPLE_GEN_H_
#define MODEL_SERVER_SRC_POPULATION_SAMPLE_GEN_H_

#include <vector>
#include "model_server/src/engine/sample.h"
#include "model_server/src/population/model_spec.h"

namespace model_server {

void random_sample_gen(
  const model_server::ModelMeta& model_meta,
  std::vector<model_server::Sample> *samples,
  int32_t sample_count,
  int32_t batch_size
);  // NOLINT

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_POPULATION_SAMPLE_GEN_H_
