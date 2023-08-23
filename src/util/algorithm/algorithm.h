// Copyright 2021 zh.luxu1986@gmail.com

#ifndef UTIL_ALGORITHM_H_
#define UTIL_ALGORITHM_H_

#include <vector>

namespace WORKSPACE::util::algorithm {

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

}  // namespace WORKSPACE::util::algorithm

#endif  // UTIL_ALGORITHM_H_
