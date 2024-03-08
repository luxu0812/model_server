// Copyright (C) 2021 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_UTIL_OS_SEMAPHORE_H_
#define MODEL_SERVER_SRC_UTIL_OS_SEMAPHORE_H_

#include <stdint.h>
#include <semaphore.h>

class Semaphore {
 public:
  Semaphore() = delete;
  Semaphore(const Semaphore&) = delete;

  explicit Semaphore(uint32_t value);
  ~Semaphore();

  void post();
  void wait();
  bool try_wait();

 private:
#ifdef __APPLE__
  sem_t *sem_;
#elif __linux__
  sem_t sem_;
#endif
};

#endif  // MODEL_SERVER_SRC_UTIL_OS_SEMAPHORE_H_
