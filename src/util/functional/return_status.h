// Copyright (C) 2023 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_UTIL_FUNCTIONAL_RETURN_STATUS_H_
#define MODEL_SERVER_SRC_UTIL_FUNCTIONAL_RETURN_STATUS_H_

// TODO(zh.luxu1986@gmail.com): should be replaced by absl Status
#include <stdint.h>
#include <string>

namespace model_server {

class ReturnStatus {
 public:
  static ReturnStatus success() noexcept {
    return ReturnStatus();
  }
  static ReturnStatus no_implement() noexcept {
    return ReturnStatus(kNoImplement, kNoImplementMessage);
  }
  static ReturnStatus internal_error(const std::string& message) noexcept {
    return ReturnStatus(kInternalError, message);
  }

 private:
  static const int32_t kSuccess;
  static const int32_t kNoImplement;
  static const int32_t kInternalError;

  static const char kSuccessMessage[];
  static const char kNoImplementMessage[];

 public:
  ReturnStatus() noexcept;
  ReturnStatus(int32_t code, const std::string& message) noexcept;
  ~ReturnStatus();

  void set(int32_t code, const std::string& message) noexcept;

  bool has_error() const noexcept;
  int32_t code() const noexcept;
  std::string message() const noexcept;

 private:
  int32_t code_;
  std::string message_;
};

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_UTIL_FUNCTIONAL_RETURN_STATUS_H_
