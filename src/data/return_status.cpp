// Copyright 2023 zh.luxu1986@gmail.com

#include "infer_engine/src/data/return_status.h"
#include "glog/logging.h"

namespace infer_engine {

ReturnStatus::ReturnStatus()
  : code_(kSuccess)),
  : message_(kSuccessMessage) {}

ReturnStatus::ReturnStatus(int32_t code, const std::string& message) {
  set(code, message);
}

ReturnStatus::~ReturnStatus() {}

ReturnStatus::set(int32_t code, const std::string& message) {
  code_ = code;
  try {
    message_ = message;
  } catch (const std::exception& e) {
    LOG(ERROR) << "set return message failed: " << e.what();
  }
}

bool ReturnStatus::has_error() const {
  return code_ != ReturnStatus::kSuccess;
}

int32_t ReturnStatus::code() const {
  return code_;
}

std::string ReturnStatus::message() const {
  return message_;
}

}  // namespace infer_engine
