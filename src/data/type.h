// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_DATA_TYPE_H_
#define INFER_ENGINE_SRC_DATA_TYPE_H_

#include <stdint.h>
#include <vector>
#include <string>

namespace infer_engine {

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

}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_DATA_TYPE_H_
