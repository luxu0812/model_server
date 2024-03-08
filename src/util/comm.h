// Copyright (C) 2021 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_UTIL_COMM_H_
#define MODEL_SERVER_SRC_UTIL_COMM_H_

#include <string>

namespace model_server {

bool execute(const std::string& cmd);

bool execute(
  const std::string& cmd,
  std::string *stdo,
  bool include_stderr = false
); // NOLINT

bool execute(
  const std::string& cmd,
  std::string *stdo,
  std::string *stde
); // NOLINT

bool execute_vfork(
  const std::string& cmd,
  std::string *res = nullptr,
  int32_t buffer_size = 0
); // NOLINT

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_UTIL_COMM_H_
