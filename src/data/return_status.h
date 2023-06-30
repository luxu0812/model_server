// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_DATA_RETURN_STATUS_H_
#define INFER_ENGINE_SRC_DATA_RETURN_STATUS_H_

#include <stdint.h>
#include <string>

namespace infer_engine {

class ReturnStatus {
 public:
  static ReturnStatus success() {
    return ReturnStatus();
  }
  static ReturnStatus no_implement() {
    return ReturnStatus(kNoImplement, kNoImplementMessage);
  }
  static ReturnStatus internal_error(const std::string& message) {
    return ReturnStatus(kInternalError, message);
  }

 private:
  static const int32_t kSuccess;
  static const int32_t kNoImplement;
  static const int32_t kInternalError;

  static const char kSuccessMessage[];
  static const char kNoImplementMessage[];

 public:
  ReturnStatus();
  ReturnStatus(int32_t code, const std::string& message);
  ~ReturnStatus();

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
