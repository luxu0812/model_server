// Copyright 2023 zh.luxu1986@gmail.com

#include "model_server/src/population/sample_gen.h"
#include <vector>
#include <random>
#include <algorithm>

namespace model_server {

void random_sample_gen(
  const model_server::ModelMeta& model_meta,
  std::vector<model_server::Sample> *samples,
  int32_t sample_count,
  int32_t batch_size
) {
  if (nullptr == samples) {
    return;
  }

  std::random_device rd;
  std::mt19937 gen(rd());

  samples->resize(sample_count);
  for (auto& sample : *samples) {
    sample.instance.batch_size = batch_size;

    sample.instance.features.resize(model_meta.input_shapes.size());
    int32_t i = 0;
    for (auto& input : model_meta.input_shapes) {
      auto& feature = sample.instance.features[i++];
      int64_t data_size = batch_size;
      for (auto& dim : input.second) {
        data_size *= dim;
      }
      feature.name = input.first;
      feature.data.resize(data_size);

      // fill random data
      for (auto& data : feature.data) {
        data = std::uniform_real_distribution<float>(0.0, 1.0)(gen);
      }
    }

    sample.score.targets.resize(model_meta.output_shapes.size());
  }
}

}  // namespace model_server
