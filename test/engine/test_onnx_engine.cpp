// Copyright 2023 zh.luxu1986@gmail.com

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "infer_engine/src/util/process/process_initiator.h"
#include "infer_engine/src/engine/onnx_engine.h"

TEST(ONNXEngine, LoadSuccess) {
  infer_engine::ModelSpec onnx_model_spec {
    .name = "model1",
    .version = "1.0.0",
    .graph_file = "test/data/model1/graph.onnx",
    .meta_file = "test/data/model1/graph_meta.json"
  };
  ASSERT_NO_THROW({ infer_engine::ONNXEngine onnx_engine(onnx_model_spec); });
}

TEST(ONNXEngine, LoadFailNonExistentGraph) {
  infer_engine::ModelSpec onnx_model_spec {
    .name = "model1",
    .version = "1.0.0",
    .graph_file = "test/data/model1/non-existent.onnx",
    .meta_file = "test/data/model1/graph_meta.json"
  };
  ASSERT_ANY_THROW({ infer_engine::ONNXEngine onnx_engine(onnx_model_spec); });
}

TEST(ONNXEngine, LoadFailNonExistentMeta) {
  infer_engine::ModelSpec onnx_model_spec {
    .name = "model1",
    .version = "1.0.0",
    .graph_file = "test/data/model1/graph.onnx",
    .meta_file = "test/data/model1/non-existent.json"
  };
  ASSERT_THROW({ infer_engine::ONNXEngine onnx_engine(onnx_model_spec); }, std::runtime_error);
}

int main (int argc, char **argv) {
  infer_engine::init(argc, argv);
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}