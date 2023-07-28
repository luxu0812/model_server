// Copyright 2023 zh.luxu1986@gmail.com

#include "model_server/src/data/return_status.h"
#include "glog/logging.h"

namespace model_server {

const int32_t ReturnStatus::kSuccess = 0;
const int32_t ReturnStatus::kNoImplement = 1;
const int32_t ReturnStatus::kInternalError = 1029;

const char ReturnStatus::kSuccessMessage[] = "success";
const char ReturnStatus::kNoImplementMessage[] = "no implement";

ReturnStatus::ReturnStatus() :
  code_(kSuccess),
  message_(kSuccessMessage) {}

ReturnStatus::ReturnStatus(int32_t code, const std::string& message) {
  set(code, message);
}

ReturnStatus::~ReturnStatus() {}

void ReturnStatus::set(int32_t code, const std::string& message) {
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

}  // namespace model_server
