// Copyright 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_LIFECYCLE_LIFECYCLE_H_
#define MODEL_SERVER_SRC_LIFECYCLE_LIFECYCLE_H_

#include <string>
#include "model_server/src/engine/engine.h"

namespace model_server {

class Lifecycle {
 public:
  Lifecycle() = default;
  virtual ~Lifecycle() = default;

  Engine& operator=(const Engine&) = delete;
  Engine(const Engine&) = delete;

  void age() {};

 private:
  std::vector<std::string> memories_;
  std::string              age_;
  Engine*                  engine_;
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_LIFECYCLE_LIFECYCLE_H_