// Copyright (C) 2023 zh.luxu1986@gmail.com
// TODO(zh.luxu1986@gmail.com): replace this with absl::Cleanup

#ifndef MODEL_SERVER_SRC_UTIL_FUNCTIONAL_SCOPE_EXIT_TASK_H_
#define MODEL_SERVER_SRC_UTIL_FUNCTIONAL_SCOPE_EXIT_TASK_H_

namespace model_server {

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

}  // namespace model_server

#endif  // MODEL_SERVER_SRC_UTIL_FUNCTIONAL_SCOPE_EXIT_TASK_H_
