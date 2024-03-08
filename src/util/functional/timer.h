// Copyright (C) 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_UTIL_FUNCTIONAL_TIMER_H_
#define MODEL_SERVER_SRC_UTIL_FUNCTIONAL_TIMER_H_

#include "absl/time/clock.h"
#include "absl/time/time.h"

namespace model_server {

class Timer {
 public:
  Timer() : start_(absl::Now()) {}

  int64_t i64_elapsed_ns() const {
    return absl::ToInt64Nanoseconds(absl::Now() - start_);
  }
  int64_t i64_elapsed_us() const {
    return absl::ToInt64Microseconds(absl::Now() - start_);
  }
  int64_t i64_elapsed_ms() const {
    return absl::ToInt64Milliseconds(absl::Now() - start_);
  }
  int64_t i64_elapsed_sec() const {
    return absl::ToInt64Seconds(absl::Now() - start_);
  }

  double f64_elapsed_ns() const {
    return absl::ToDoubleNanoseconds(absl::Now() - start_);
  }
  double f64_elapsed_us() const {
    return absl::ToDoubleMicroseconds(absl::Now() - start_);
  }
  double f64_elapsed_ms() const {
    return absl::ToDoubleMilliseconds(absl::Now() - start_);
  }
  double f64_elapsed_sec() const {
    return absl::ToDoubleSeconds(absl::Now() - start_);
  }

 private:
  absl::Time start_;
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_UTIL_FUNCTIONAL_TIMER_H_
