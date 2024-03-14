// Copyright (C) 2023 zh.luxu1986@gmail.com

#include <stdint.h>
#include <exception>
#include <vector>

#include "absl/log/log.h"
#include "BShoshany/BS_thread_pool.hpp"

#include "model_server/src/util/process/process_initiator.h"
#include "model_server/src/util/functional/timer.h"
#include "model_server/src/config/gflags.h"
#include "model_server/src/engine/sample.h"
#include "model_server/src/engine/engine.h"
#include "select_engine.h"  // NOLINT

model_server::Engine *create_engine();

int main(int argc, char **argv) {
  model_server::init(argc, argv);
  try {
    std::unique_ptr<model_server::Engine> engine(create_demo_engine_3());
    if (nullptr == engine.get()) {
      LOG(ERROR) << "Failed to create engine";
      return -1;
    }

    model_server::PerfIndex perf_index;
    engine->perf(
      absl::GetFlag(FLAGS_number_of_consumers),
      absl::GetFlag(FLAGS_number_of_test_cases),
      absl::GetFlag(FLAGS_batch_size), &perf_index
    );  //  NOLINT
    LOG(INFO) << "Summary:\n" << perf_index.DebugString();
  } catch (const std::exception& e) {
    LOG(ERROR) << e.what();
  } catch (...) {
    LOG(ERROR) << "Unknown exception";
  }
  LOG(INFO) << "Done";

  return 0;
}
