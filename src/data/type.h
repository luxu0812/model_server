// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_DATA_TYPE_H_
#define INFER_ENGINE_SRC_DATA_TYPE_H_

#include <stdint.h>
#include <vector>
#include <string>

namespace infer_engine {

struct Tensor {
  std::string name;
  std::vector<int32_t> shape;
  std::vector<float> data;
};

struct Instance {
  std::vector<Tensor> features;
};

struct Score {
  std::vector<Tensor> targets;
};

struct BatchInstance {
  std::vector<Instance> instances;
};

struct BatchScore {
  std::vector<Score> scores;
};

}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_DATA_TYPE_H_
