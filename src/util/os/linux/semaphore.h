// Copyright 2021 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_UTIL_OS_LINUX_SEMAPHORE_H_
#define MODEL_SERVER_SRC_UTIL_OS_LINUX_SEMAPHORE_H_

#include <stdio.h>
#include <semaphore.h>
#include <type_traits>

template<class FUNC, class... ARGS>
static auto ignore_signal_call(
  FUNC func, ARGS && ... args
) -> typename std::result_of<FUNC(ARGS...)>::type {
  for (;;) {
    auto err = func(args...);

    if (err < 0 && errno == EINTR) {
      fprintf(stderr, "Signal is caught. Ignored.");
      continue;
    }

    return err;
  }
}

class Semaphore {
 public:
  Semaphore() = delete;
  Semaphore(const Semaphore&) = delete;

  explicit Semaphore(uint32_t value) {
    if (0 != sem_init(&sem_, 0, value)) {
      fprintf(stderr, "Fail to init semaphore.");
      exit(EXIT_FAILURE);
    }
  }
  ~Semaphore() {
    if (0 != sem_destroy(&sem_)) {
      fprintf(stderr, "Fail to destroy semaphore.");
      exit(EXIT_FAILURE);
    }
  }

  void post() {
    if (0 != sem_post(&sem_)) {
      fprintf(stderr, "Fail to post semaphore.");
      exit(EXIT_FAILURE);
    }
  }
  void wait() {
    if (0 != ignore_signal_call(sem_wait, &sem_)) {
      fprintf(stderr, "Fail to wait semaphore.");
      exit(EXIT_FAILURE);
    }
  }
  bool try_wait() {
    int32_t err = ignore_signal_call(sem_trywait, &sem_);
    if (err != 0 && errno != EAGAIN) {
      fprintf(stderr, "Fail to trywait semaphore.");
      exit(EXIT_FAILURE);
    }
    return 0 == err;
  }

 private:
  sem_t sem_;
};

// #include <mutex>
// #include <condition_variable>
//
// class Semaphore {
//  public:
//   Semaphore() : count(1) {}
//   explicit Semaphore(int value) : count(value) {}
//   void down() {
//     std::unique_lock<std::mutex> lck(mtk);
//     if (--count < 0) {
//       cv.wait(lck);
//     }
//   }
//   void up() {
//     std::unique_lock<std::mutex> lck(mtk);
//     if (++count <= 0) {
//       cv.notify_one();
//     }
//   }
//  private:
//   int count;
//   std::mutex mtk;
//   std::condition_variable cv;
// };

#endif  // MODEL_SERVER_SRC_UTIL_OS_LINUX_SEMAPHORE_H_
