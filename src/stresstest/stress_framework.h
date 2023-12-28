// Copyright 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_STRESSTEST_STRESS_FRAMEWORK_H_
#define MODEL_SERVER_SRC_STRESSTEST_STRESS_FRAMEWORK_H_

#include <stdint.h>
#include <exception>
#include <memory>
#include <vector>
#include <algorithm>

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "BShoshany/BS_thread_pool.hpp"

#include "model_server/src/util/functional/timer.h"
#include "model_server/src/util/os/resource_used.h"
#include "model_server/src/engine/sample.h"
#include "model_server/src/engine/engine.h"

static void infer(model_server::Engine *engine, model_server::Sample *sample, double *cost_ms) {
  model_server::Timer timer;
  model_server::ScopeExitTask scope_exit_task([&]() {
    *cost_ms = timer.f64_elapsed_ms();
  });

  try {
    engine->infer(&sample->instance, &sample->score);
  } catch (const std::exception& e) {
    LOG(ERROR) << e.what();
  } catch (...) {
    LOG(ERROR) << "Unknown exception";
  }
}

class StressFramework {
 public:
  StressFramework() = delete;
  explicit StressFramework(model_server::Engine *engine) noexcept(false) :
    engine_(engine) {
  }
  ~StressFramework() {}

  StressFramework& operator=(const StressFramework&) = delete;
  StressFramework(const StressFramework&) = delete;

  void run(std::vector<model_server::Sample> *samples, int32_t concurrency) {
    try {
      std::vector<double> cost_ms(samples->size());
      for (int32_t i = 0; i < samples->size(); ++i) {
        infer(engine_, &(samples->at(i)), &(cost_ms[i]));
      }

      BS::thread_pool works(concurrency);

      struct ResourceUsed resource_base, resource_curr;
      get_process_resource_used(&resource_base);
      model_server::Timer timer;
      for (int32_t i = 0; i < samples->size(); ++i) {
        works.push_task(infer, engine_, &(samples->at(i)), &(cost_ms[i]));
      }
      works.wait_for_tasks();
      double total_cost_sec = timer.f64_elapsed_sec();
      get_process_resource_used(&resource_curr);

      std::sort(cost_ms.begin(), cost_ms.end());
      double cost_avg = std::accumulate(cost_ms.begin(), cost_ms.end(), 0.0) / cost_ms.size();
      double cost_p99 = cost_ms[static_cast<int32_t>(cost_ms.size() * 0.99)];
      LOG(INFO) << "Total cost: " << total_cost_sec << " sec, throughput: "
                << static_cast<double>(samples->size()) * static_cast<double>(samples->at(0).instance.batch_size)
                 / total_cost_sec
                << ", avg cost: " << cost_avg << " ms, p99 cost: " << cost_p99 << " ms"
                << ", cpu used: " <<
                 ((resource_curr.user_time - resource_base.user_time) +
                  (resource_curr.system_time - resource_base.system_time)) / total_cost_sec
                << " cc, mem used: " << resource_curr.resident_mb << " MB";
    } catch (const std::exception& e) {
      LOG(ERROR) << e.what();
    } catch (...) {
      LOG(ERROR) << "Unknown exception";
    }
  }

 private:
  model_server::Engine *engine_;
};

#endif  // MODEL_SERVER_SRC_STRESSTEST_STRESS_FRAMEWORK_H_
