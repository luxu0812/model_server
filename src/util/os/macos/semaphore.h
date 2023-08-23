// Copyright 2021 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_UTIL_OS_MACOS_SEMAPHORE_H_
#define MODEL_SERVER_SRC_UTIL_OS_MACOS_SEMAPHORE_H_

#include <stdio.h>
#include <semaphore.h>
#include <type_traits>

static const char kSemaphorePath[] = "/semaphore";

template <typename FUNC, typename... ARGS>
static auto ignore_signal_call(
  FUNC func, ARGS &&... args
) -> std::invoke_result_t<FUNC, ARGS...> {
    for (;;) {
        auto err = std::invoke(func, args...);

        if (err < 0 && errno == EINTR) {
            std::cout << "Signal is caught. Ignored." << std::endl;
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
    if ((sem_ = sem_open(kSemaphorePath, O_CREAT, 0644, value)) == SEM_FAILED) {
      fprintf(stderr, "Fail to open semaphore.");
      exit(EXIT_FAILURE);
    }
  }
  ~Semaphore() {
    if (sem_close(sem_) != 0) {
      fprintf(stderr, "Fail to close semaphore.");
      exit(EXIT_FAILURE);
    }
    if (sem_unlink(kSemaphorePath) != 0) {
      fprintf(stderr, "Fail to unlink semaphore.");
      exit(EXIT_FAILURE);
    }
  }

  void post() {
    if (sem_post(sem_) != 0) {
      fprintf(stderr, "Fail to post semaphore.");
      exit(EXIT_FAILURE);
    }
  }
  void wait() {
    if (ignore_signal_call(sem_wait, sem_) != 0) {
      fprintf(stderr, "Fail to wait semaphore.");
      exit(EXIT_FAILURE);
    }
  }

  bool try_wait() {
    int err = ignore_signal_call(sem_trywait, sem_);
    if (0 != err && EAGAIN != errno) {
      fprintf(stderr, "Fail to trywait semaphore.");
      exit(EXIT_FAILURE);
    }
    return 0 == err;
  }

 private:
  sem_t *sem_;
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

#endif  // MODEL_SERVER_SRC_UTIL_OS_MACOS_SEMAPHORE_H_
