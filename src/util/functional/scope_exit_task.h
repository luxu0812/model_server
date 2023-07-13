// Copyright 2023 zh.luxu1986@gmail.com

#ifndef INFER_ENGINE_SRC_UTIL_FUNCTIONAL_SCOPE_EXIT_TASK_H_
#define INFER_ENGINE_SRC_UTIL_FUNCTIONAL_SCOPE_EXIT_TASK_H_

namespace infer_engine {

class ScopeExitTask {
 public:
  ScopeExitTask() = delete;
  explicit ScopeExitTask(std::function<void()> func) :
    func_(func) {
  }
  ~ScopeExitTask() {
    func_();
  }
  ScopeExitTask& operator=(const ScopeExitTask&) = delete;
  ScopeExitTask(const ScopeExitTask&) = delete;

 private:
  std::function<void()> func_;
};

}  // namespace infer_engine

#endif  // INFER_ENGINE_SRC_UTIL_FUNCTIONAL_SCOPE_EXIT_TASK_H_
