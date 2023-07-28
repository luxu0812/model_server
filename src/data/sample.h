// Copyright 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_DATA_SAMPLE_H_
#define MODEL_SERVER_SRC_DATA_SAMPLE_H_

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

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_DATA_SAMPLE_H_
