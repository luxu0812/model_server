// Copyright 2023 zh.luxu1986@gmail.com

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "infer_engine/src/engine/tf_engine.h"

TEST(TFEngine, Load) {
  try {
    infer_engine::ModelSpec tf_model_spec {
      .name = "test",
      .version = "1.0.0",
      .graph_file = "test/data/model1/graph.pb",
      .meta_file = "test/data/model1/graph_meta.json"
    };
    infer_engine::TFEngine tf_engine(tf_model_spec);
  } catch (const std::exception& e) {
    LOG(ERROR) << e.what();
  } catch (...) {
    LOG(ERROR) << "Unknown exception";
  }
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
