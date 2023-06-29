// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_DATA_RETURN_STATUS_H_
#define INFER_ENGINE_SRC_DATA_RETURN_STATUS_H_

#include <stdint.h>
#include <string>

namespace infer_engine {

class RetrunStatus {
 public:
  static RetrunStatus success() {
    return RetrunStatus();
  }
  static ReturnStatus no_implement() {
    return ReturnStatus(kNoImplement, kNoImplementMessage);
  }
  static ReturnStatus internal_error(const std::string& message) {
    return ReturnStatus(kInternalError, message);
  }

 private:
  static const int32_t kSuccess = 0;
  static const int32_t kNoImplement = 1;
  static const int32_t kInternalError = 1029;

  static const char kSuccessMessage[] = "success";
  static const char kNoImplementMessage[] = "no implement";

 public:
  RetrunStatus();
  ReturnStatus(int32_t code, const std::string& message);
  ~RetrunStatus();

  void set(int32_t code, const std::string& message);

  bool has_error() const;
  int32_t code() const;
  std::string message() const;

 private:
  int32_t code_;
  std::string message_;
};

}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_DATA_RETURN_STATUS_H_
