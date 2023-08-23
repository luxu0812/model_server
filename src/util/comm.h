// Copyright 2021 zh.luxu1986@gmail.com

#ifndef UTIL_COMM_H_
#define UTIL_COMM_H_

#include <string>

namespace WORKSPACE::util::comm {

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

}  // namespace WORKSPACE::util::comm

#endif  // UTIL_COMM_H_
