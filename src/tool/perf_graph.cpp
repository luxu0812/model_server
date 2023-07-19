// Copyright 2023 zh.luxu1986@gmail.com

#include "gflags/gflags.h"
#include "glog/logging.h"

#include "infer_engine/src/util/process/process_initiator.h"
#include "infer_engine/src/data/model_spec.h"
#include "infer_engine/src/engine/engine.h"
#include "infer_engine/src/engine/tf_engine.h"
#include "infer_engine/src/engine/onnx_engine.h"

int main(int argc, char **argv) {
  infer_engine::init(argc, argv);

  try {
    infer_engine::ModelSpec tf_model_spec {
      .name = "test",
      .version = "1.0.0",
      .graph_file = "test/data/model1/graph.pb",
      .meta_file = "test/data/model1/graph_meta.json"
    };
    infer_engine::TFEngine tf_engine(tf_model_spec);

    infer_engine::ModelSpec onnx_model_spec {
      .name = "test",
      .version = "1.0.0",
      .graph_file = "test/data/model1/graph.onnx",
      .meta_file = "test/data/model1/graph_meta.json"
    };
    infer_engine::ONNXEngine onnx_engine(onnx_model_spec);
  } catch (const std::exception& e) {
    LOG(ERROR) << e.what();
  } catch (...) {
    LOG(ERROR) << "Unknown exception";
  }

  return 0;
}
