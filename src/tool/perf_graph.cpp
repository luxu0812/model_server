// Copyright 2023 zh.luxu1986@gmail.com

#include "gflags/gflags.h"
#include "glog/logging.h"

#include "infer_engine/src/data/model_spec.h"
#include "infer_engine/src/engine/engine.h"
#include "infer_engine/src/engine/tf_engine.h"
#include "infer_engine/src/engine/onnx_engine.h"

int main(int argc, char **argv) {
  google::AllowCommandLineReparsing();
  google::ParseCommandLineFlags(&argc, &argv, true);

  FLAGS_logbufsecs = 0;
  FLAGS_max_log_size = 1024;
  FLAGS_minloglevel = google::INFO;
  FLAGS_logtostdout = true;
  google::InitGoogleLogging(argv[0]);

  try {
    infer_engine::ModelSpec tf_model_spec {
      .name = "test",
      .version = "1.0.0",
      .graph_file = "/Users/xu.lu/Downloads/graph_final.pb"
    };
    infer_engine::TFEngine tf_engine(tf_model_spec);

    infer_engine::ModelSpec onnx_model_spec {
      .name = "test",
      .version = "1.0.0",
      .graph_file = "/Users/xu.lu/Downloads/model.onnx"
    };
    infer_engine::ONNXEngine onnx_engine(onnx_model_spec);
  } catch (const std::exception& e) {
    LOG(ERROR) << e.what();
  } catch (...) {
    LOG(ERROR) << "Unknown exception";
  }

  return 0;
}
