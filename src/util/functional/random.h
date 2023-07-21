// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_UTIL_FUNCTIONAL_RANDOM_H_
#define INFER_ENGINE_SRC_UTIL_FUNCTIONAL_RANDOM_H_

#include <random>

namespace infer_engine {

double uniform_real_distribution(double min_value, double max_value) {
  static std::random_device rd;
  static std::mt19937 gen(rd());

  // Create a uniform real distribution object with the specified range
  std::uniform_real_distribution<double> distribution(min_value, max_value);

  // Generate a random double value from the uniform distribution
  return distribution(gen);
}

double normal_distribution(double mean, double stddev) {
  static std::random_device rd;
  static std::mt19937 gen(rd());

  // Create a normal distribution object with the specified mean and stddev
  std::normal_distribution<double> distribution(mean, stddev);

  // Generate a random double value from the normal distribution
  return distribution(gen);
}

}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_UTIL_FUNCTIONAL_RANDOM_H_
