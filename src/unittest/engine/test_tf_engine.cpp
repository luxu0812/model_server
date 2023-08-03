// Copyright 2023 zh.luxu1986@gmail.com

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "model_server/src/util/process/process_initiator.h"
#include "model_server/src/engine/tf_engine.h"

model_server::RuntimeConf runtime_conf {
  .opt_level = 0,
  .jit_level = 0,
  .inter_op_parallelism_threads = 1,
  .intra_op_parallelism_threads = 1
};

TEST(TFEngine, LoadSuccess) {
  model_server::ModelSpec tf_model_spec {
    .name = "model_1",
    .version = "1.0.0",
    .graph_file = "data/models/model_1/graph.pb",
    .meta_file = "data/models/model_1/graph_meta.json"
  };
  ASSERT_NO_THROW({
    model_server::TFEngine tf_engine(tf_model_spec, runtime_conf);
  });
}

TEST(TFEngine, LoadFailNonExistentGraph) {
  model_server::ModelSpec tf_model_spec {
    .name = "model_1",
    .version = "1.0.0",
    .graph_file = "data/models/model_1/non-existent.pb",
    .meta_file = "data/models/model_1/graph_meta.json"
  };
  ASSERT_THROW({
    model_server::TFEngine tf_engine(tf_model_spec, runtime_conf);
  }, std::runtime_error);
}

TEST(TFEngine, LoadFailNonExistentMeta) {
  model_server::ModelSpec tf_model_spec {
    .name = "model_1",
    .version = "1.0.0",
    .graph_file = "data/models/model_1/graph.pb",
    .meta_file = "data/models/model_1/non-existent.json"
  };
  ASSERT_THROW({
    model_server::TFEngine tf_engine(tf_model_spec, runtime_conf);
  }, std::runtime_error);
}

int main(int argc, char **argv) {
  model_server::init(argc, argv);
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
