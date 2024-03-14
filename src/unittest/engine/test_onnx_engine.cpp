// Copyright (C) 2023 zh.luxu1986@gmail.com

#include "absl/log/log.h"
#include "gtest/gtest.h"
#include "model_server/src/util/process/process_initiator.h"
#include "model_server/src/engine/onnx_engine.h"

TEST(ONNXEngine, LoadSuccess) {
  model_server::EngineConf onnx_engine_conf {
    .name = "model_1",
    .version = "1.0.0",
    .graph_file_loc = "data/models/model_1/2/graph.onnx",
    .input_nodes = {"dense", "sparse_input_unfolded"},
    .output_nodes = {"predict_node", "p0_click", "p0_atc", "p0_order"},
    .opt_level = 0,
    .jit_level = 0,
    .inter_op_parallelism_threads = 1,
    .intra_op_parallelism_threads = 1
  };
  ASSERT_NO_THROW({
    model_server::ONNXEngine onnx_engine(onnx_engine_conf);
  });
}

TEST(ONNXEngine, LoadFailNonExistentGraph) {
  model_server::EngineConf onnx_engine_conf {
    .name = "model_1",
    .version = "1.0.0",
    .graph_file_loc = "data/models/model_1/2/non-existent.onnx",
    .input_nodes = {"dense", "sparse_input_unfolded"},
    .output_nodes = {"predict_node", "p0_click", "p0_atc", "p0_order"},
    .opt_level = 0,
    .jit_level = 0,
    .inter_op_parallelism_threads = 1,
    .intra_op_parallelism_threads = 1
  };
  ASSERT_ANY_THROW({
    model_server::ONNXEngine onnx_engine(onnx_engine_conf);
  });
}

int main(int argc, char **argv) {
  model_server::init(argc, argv);
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
