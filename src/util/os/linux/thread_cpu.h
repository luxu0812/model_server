// Copyright 2021 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_UTIL_OS_LINUX_THREAD_CPU_H_
#define MODEL_SERVER_SRC_UTIL_OS_LINUX_THREAD_CPU_H_

#include <unistd.h>
#include <pthread.h>
#include <iostream>

bool get_physical_cpu_num(int32_t *num) {
  if (nullptr == num) {
    return false;
  }
  *num = sysconf(_SC_NPROCESSORS_ONLN);
  return true;
}

bool get_thread_cpu(int64_t *cpu_id) {
  if (nullptr == cpu_id) {
    return false;
  }

  cpu_set_t cpu_mask;
  CPU_ZERO(&cpu_mask);
  int ret = pthread_getaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu_mask);
  if (0 != ret) {
    return false;
  }

  for (int i = 0; i < sysconf(_SC_NPROCESSORS_CONF); ++i) {
    if (CPU_ISSET(i, &cpu_mask)) {
      *cpu_id = i;
      return true;
    }
  }

  return false;
}

bool set_thread_cpu(int64_t cpu_id) {
  pthread_t tid = pthread_self();

  cpu_set_t cpu_set;
  CPU_ZERO(&cpu_set);
  CPU_SET(cpu_id, &cpu_set);
  int ret = pthread_setaffinity_np(tid, sizeof(cpu_set), &cpu_set);
  if (0 != ret) {
    return false;
  }

  return true;
}

#endif  // MODEL_SERVER_SRC_UTIL_OS_LINUX_THREAD_CPU_H_
