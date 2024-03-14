// Copyright (C) 2023 zh.luxu1986@gmail.com
// TODO(zh.luxu1986@gmail.com): replace this with absl::Random

#ifndef MODEL_SERVER_SRC_UTIL_FUNCTIONAL_RANDOM_H_
#define MODEL_SERVER_SRC_UTIL_FUNCTIONAL_RANDOM_H_

#include <random>

namespace model_server {

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

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_UTIL_FUNCTIONAL_RANDOM_H_
