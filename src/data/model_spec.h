// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_DATA_MODEL_SPEC_H_
#define INFER_ENGINE_SRC_DATA_MODEL_SPEC_H_

#include <stdint.h>
#include <vector>
#include <string>
#include "absl/container/flat_hash_map.h"

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

struct ModelMeta {
  absl::flat_hash_map<std::string, std::vector<int64_t>> input_shapes;
  absl::flat_hash_map<std::string, std::vector<int64_t>> output_shapes;

  void load(const std::string& meta_file) {}
};


}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_DATA_MODEL_SPEC_H_
