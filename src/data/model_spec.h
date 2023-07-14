// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_DATA_MODEL_SPEC_H_
#define INFER_ENGINE_SRC_DATA_MODEL_SPEC_H_

#include <string>

namespace infer_engine {

struct ModelSpec {
  std::string name;
  std::string version;
  std::string graph_file;
  std::string meta_file;

  std::string to_string() {
    return "name: " + name + ", version: " + version + ", graph_file: " + graph_file + ", meta_file: " + meta_file;
  }

  std::string brief() {
    return name + ":" + version;
  }
};


}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_DATA_MODEL_SPEC_H_
