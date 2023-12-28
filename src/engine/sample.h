// Copyright 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_ENGINE_SAMPLE_H_
#define MODEL_SERVER_SRC_ENGINE_SAMPLE_H_

#include <stdint.h>
#include <vector>
#include <string>

namespace model_server {

struct Tensor {
  std::string name;
  std::vector<float> data;
};

struct Instance {
  int64_t batch_size;
  std::vector<Tensor> features;
};

struct Score {
  std::vector<Tensor> targets;
};

struct Sample {
  Instance instance;
  Score score;
};

struct OptInstance {
    std::vector<char *> shared_features;
    std::vector<char *> unique_features;

    std::vector<std::vector<float>> shared_features_data;
    std::vector<std::vector<float>> unique_features_data;
};

struct OptScore {
  std::vector<char *> targets;
  std::vector<std::vector<float>> targets_data;
};

struct OptSample {
  int64_t batch_size;
  OptInstance instance;
  OptScore score;
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_ENGINE_SAMPLE_H_
