// Copyright 2023 zh.luxu1986@gmail.com

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "infer_engine/src/engine/tf_engine.h"

TEST(TFEngine, LoadSuccess) {
  infer_engine::ModelSpec tf_model_spec {
    .name = "model1",
    .version = "1.0.0",
    .graph_file = "test/data/model1/graph.pb",
    .meta_file = "test/data/model1/graph_meta.json"
  };
  ASSERT_NO_THROW({ infer_engine::TFEngine tf_engine(tf_model_spec); });
}

TEST(TFEngine, LoadFailNonExistentGraph) {
  infer_engine::ModelSpec tf_model_spec {
    .name = "model1",
    .version = "1.0.0",
    .graph_file = "test/data/model1/non-existent.pb",
    .meta_file = "test/data/model1/graph_meta.json"
  };
  ASSERT_THROW({ infer_engine::TFEngine tf_engine(tf_model_spec); }, std::runtime_error);
}

TEST(TFEngine, LoadFailNonExistentMeta) {
  infer_engine::ModelSpec tf_model_spec {
    .name = "model1",
    .version = "1.0.0",
    .graph_file = "test/data/model1/graph.pb",
    .meta_file = "test/data/model1/non-existent.json"
  };
  ASSERT_ANY_THROW({ infer_engine::TFEngine tf_engine(tf_model_spec); });
}

int main (int argc, char **argv) {
  google::AllowCommandLineReparsing();
  google::ParseCommandLineFlags(&argc, &argv, true);

  FLAGS_logbufsecs = 0;
  FLAGS_max_log_size = 1024;
  FLAGS_minloglevel = google::INFO;
  FLAGS_logtostdout = true;
  google::InitGoogleLogging(argv[0]);

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
