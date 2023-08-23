// Copyright 2021 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_UTIL_OS_MACOS_THREAD_CPU_H_
#define MODEL_SERVER_SRC_UTIL_OS_MACOS_THREAD_CPU_H_

#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/mach.h>
#include <mach/thread_policy.h>

bool get_physical_cpu_num(int32_t *num) {
  if (nullptr == num) {
    return false;
  }
  size_t size = sizeof(*num);
  if (-1 == sysctlbyname("hw.physicalcpu", num, &size, nullptr, 0)) {
    return false;
  }
  return true;
}

bool get_thread_cpu(int64_t *cpu_id) {
  if (nullptr == cpu_id) {
    return false;
  }

  thread_port_t thread_port = pthread_mach_thread_np(pthread_self());

  struct thread_affinity_policy policy;
  policy.affinity_tag = 0;
  mach_msg_type_number_t count = THREAD_AFFINITY_POLICY_COUNT;
  boolean_t get_default = FALSE;

  kern_return_t ret = thread_policy_get(
    thread_port, THREAD_AFFINITY_POLICY, (thread_policy_t)&policy,
    &count, &get_default
  );  // NOLINT
  if (KERN_SUCCESS != ret) {
    return false;
  }

  *cpu_id = policy.affinity_tag;
  return true;
}

bool set_thread_cpu(int64_t cpu_id) {
  thread_port_t thread_port = pthread_mach_thread_np(pthread_self());

  struct thread_affinity_policy policy;
  policy.affinity_tag = cpu_id;

  kern_return_t ret = thread_policy_set(
    thread_port, THREAD_AFFINITY_POLICY, (thread_policy_t)&policy,
    THREAD_AFFINITY_POLICY_COUNT
  );  // NOLINT
  if (KERN_SUCCESS != ret) {
    return false;
  }

  return true;
}

#endif  // MODEL_SERVER_SRC_UTIL_OS_MACOS_THREAD_CPU_H_
