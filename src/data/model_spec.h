// Copyright 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_DATA_MODEL_SPEC_H_
#define MODEL_SERVER_SRC_DATA_MODEL_SPEC_H_

#include <stdint.h>
#include <fstream>
#include <vector>
#include <string>
#include "absl/container/flat_hash_map.h"
#include "nlohmann/json.hpp"
#include "model_server/src/util/functional/scope_exit_task.h"

namespace model_server {

static const char kFeatureTypeCategorical[]          = "sparse";
static const char kFeatureTypeContinuous[]           = "dense";
static const char kAggregatorMean[]                  = "mean";
static const char kAggregatorMadian[]                = "median";
static const char kAggregatorSum[]                   = "sum";
static const char kAggregatorCount[]                 = "count";
static const char kAggregatorMin[]                   = "min";
static const char kAggregatorMax[]                   = "max";
static const char kAggregatorMode[]                  = "mode";
static const char kAggregatorVariance[]              = "variance";
static const char kAggregatorStandardDeviation[]     = "standard deviation";
static const char kAggregatorQuantile[]              = "quantile";
static const char kAggregatorWeightedMean[]          = "weighted mean";
static const char kInputFeatureTypeFieldName[]       = "type";
static const char kInputFeatureSpecisesFieldName[]   = "slot";
static const char kInputFeatureAggregatorFieldName[] = "combiner";
static const char kInputFeatureDimFieldName[]        = "dim";
static const char kInputFeatureOffsetFieldName[]     = "optimized_offset";
static const char kInputFieldName[]                  = "optimized_inputs";
static const char kInputNameFieldName[]              = "name";
static const char kInputShapeFieldName[]             = "dim";
static const char kInputFeatureFieldName[]           = "input_tensors";
static const char kOutputFieldName[]                 = "outputs";

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

struct FeatureMeta{
  std::string type;
  std::string aggregator;
  int32_t specises;
  int32_t dim;
  int32_t offset;
};

struct ModelMeta {
  absl::flat_hash_map<std::string, std::vector<int32_t>> output_shapes;
  absl::flat_hash_map<std::string, std::vector<int32_t>> input_shapes;
  absl::flat_hash_map<std::string, std::vector<FeatureMeta>> input_features;
  std::string json_file;
  nlohmann::json conf;

  void load(const std::string& meta_file) {
    std::ifstream file(meta_file);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open meta file: " + meta_file);
    }
    ScopeExitTask file_guard([&file](){ file.close(); });

    conf = nlohmann::json::parse(file);
    check_format();
    parse_output();
    parse_input();
  }

 private:
  void check_format() {
    if ((!conf.contains(kInputFieldName)) || (!conf[kInputFieldName].is_array())) {
      const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
        + json_file + "] " + "kInputFieldName format error, " + conf.dump();
      throw std::runtime_error(err_msg);
    }
    if ((!conf.contains(kOutputFieldName)) || (!conf[kOutputFieldName].is_array())) {
      const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
        + json_file + "] " + "kOutputFieldName format error, " + conf.dump();
      throw std::runtime_error(err_msg);
    }

    const auto& inputs = conf[kInputFieldName];
    for (const auto& input : inputs) {
      if ((!input.contains(kInputNameFieldName)) || (!input.contains(kInputShapeFieldName))) {
        const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
          + json_file + "] " + "kInputNameFieldName or kInputShapeFieldName format error, " + input.dump();
        throw std::runtime_error(err_msg);
      }

      const auto& shape = input[kInputShapeFieldName];
      if (shape.is_array()) {
        for (const auto& dim : shape) {
          if (!dim.is_number()) {
            const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
              + json_file + "] " + "shape field format error, " + shape.dump();
            throw std::runtime_error(err_msg);
          }
        }
      } else if (shape.is_number()) {
      } else {
        const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
          + json_file + "] " + "shape field format error, " + shape.dump();
        throw std::runtime_error(err_msg);
      }

      if (input.contains(kInputFeatureFieldName)) {
        const auto& features = input[kInputFeatureFieldName];
        if (!features.is_array()) {
          const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
            + json_file + "] " + "kInputFeatureFieldName format error, " + features.dump();
          throw std::runtime_error(err_msg);
        }
        for (const auto& feature : features) {
          if (!feature.contains(kInputFeatureTypeFieldName)) {
            const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
              + json_file + "] " + "kInputFeatureTypeFieldName format error, " + feature.dump();
            throw std::runtime_error(err_msg);
          }
          if (!feature.contains(kInputFeatureAggregatorFieldName)) {
            const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
              + json_file + "] " + "kInputFeatureAggregatorFieldName format error, " + feature.dump();
            throw std::runtime_error(err_msg);
          }
          if ((!feature.contains(kInputFeatureSpecisesFieldName)) ||
              (!feature[kInputFeatureSpecisesFieldName].is_number())) {
            const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
              + json_file + "] " + "kInputFeatureSpecisesFieldName format error, " + feature.dump();
            throw std::runtime_error(err_msg);
          }
          if ((!feature.contains(kInputFeatureDimFieldName)) ||
              (!feature[kInputFeatureDimFieldName].is_number())) {
            const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
              + json_file + "] " + "kInputFeatureDimFieldName format error, " + feature.dump();
            throw std::runtime_error(err_msg);
          }
          if ((!feature.contains(kInputFeatureOffsetFieldName)) ||
              (!feature[kInputFeatureOffsetFieldName].is_number())) {
            const std::string& err_msg = "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]["
              + json_file + "] " + "kInputFeatureOffsetFieldName format error, " + feature.dump();
            throw std::runtime_error(err_msg);
          }
        }
      }
    }
  }

  void parse_output() {
    const auto& outputs = conf[kOutputFieldName];
    for (const auto& output : outputs) {
      output_shapes.insert({output, std::vector<int32_t>()});
    }
  }

  void parse_input() {
    const auto& inputs = conf[kInputFieldName];
    for (const auto& input : inputs) {
      const std::string& name = input[kInputNameFieldName];

      input_shapes[name] = std::vector<int32_t>();
      const auto& shape = input[kInputShapeFieldName];
      if (shape.is_array()) {
        for (const auto& dim : shape) {
          input_shapes[name].push_back(dim.get<int32_t>());
        }
      } else if (shape.is_number()) {
        input_shapes[name].push_back(shape.get<int32_t>());
      }

      if (input.contains(kInputFeatureFieldName)) {
        input_features[name] = std::vector<FeatureMeta>();
        const auto& features = input[kInputFeatureFieldName];
        for (const auto& feature : features) {
          input_features[name].push_back(FeatureMeta{
            .type       = feature[kInputFeatureTypeFieldName].get<std::string>(),
            .aggregator = feature[kInputFeatureAggregatorFieldName].get<std::string>(),
            .specises   = feature[kInputFeatureSpecisesFieldName].get<int32_t>(),
            .dim        = feature[kInputFeatureDimFieldName].get<int32_t>(),
            .offset     = feature[kInputFeatureOffsetFieldName].get<int32_t>(),
          });  // NOLINT
        }
      }
    }
  }
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_DATA_MODEL_SPEC_H_
