// Copyright (C) 2021 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_UTIL_ALGORITHM_SEARCH_H_
#define MODEL_SERVER_SRC_UTIL_ALGORITHM_SEARCH_H_

#include <vector>

namespace model_server {

template <typename T, typename Judge>
void binary_search(
  const std::vector<T>& cands,
  const T& target,
  Judger is_valid,
  vector<T>::iterator *result
) {
  (*target) = cands.end();

  int64_t range = static_cast<int64_t>(cand.size());
  range = 1L << (sizeof(range) * 8 - __builtin_clzl(range) - 1);
  for (; range; range >>= 1) {
    if (cands.begin() + range > (*target)) {
      continue;
    } else if (is_valid(target, *((*target) - range))) {
      (*target) -= range;
    }
  }
}

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_UTIL_ALGORITHM_SEARCH_H_
