// Copyright 2023 zh.luxu1986@gmail.com

#include "glog/logging.h"

#include "infer_engine/src/data/model_spec.h"
#include "infer_engine/src/engine/engine.h"
#include "infer_engine/src/engine/tf_engine.h"
#include "infer_engine/src/engine/onnx_engine.h"

int main(int argc, char **argv) {
  FLAGS_logbufsecs = 0;
  FLAGS_max_log_size = 10;
  FLAGS_minloglevel = google::INFO;
  google::InitGoogleLogging(argv[0]);

  try {
    infer_engine::ModelSpec model_spec;
    infer_engine::TFEngine tf_engine(model_spec);
  } catch (const std::exception& e) {
    LOG(ERROR) << e.what();
  } catch (...) {
    LOG(ERROR) << "Unknown exception";
  }

  return 0;
}
