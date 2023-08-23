// Copyright 2021 zh.luxu1986@gmail.com

#ifndef MODEL_SERVER_SRC_UTIL_OS_LINUX_RESOURCE_USED_H_
#define MODEL_SERVER_SRC_UTIL_OS_LINUX_RESOURCE_USED_H_

#include <time.h>
#include <unistd.h>
#include <string>
#include <ios>
#include <fstream>

using std::string;
using std::ifstream;
using std::ios_base;

struct ResourceUsed {
  double resident_mb;
  double user_time;    // in seconds
  double system_time;  // in seconds
};

bool get_process_resource_used(struct ResourceUsed *res) {
  if (nullptr == res) {
    return false;
  }

  // useless fields
  string pid, comm, state, ppid, pgrp, session, tty_nr,
         tpgid, flags, minflt, cminflt, majflt, cmajflt,
         cutime, cstime, priority, nice, num_threads,
         itrealvalue, starttime, rsslim, startcode, endcode,
         startstack, kstkesp, kstkeip, signal, blocked,
         sigignore, sigcatch, wchan, nswap, cnswap, exit_signal,
         processor, rt_priority, policy, delayacct_blkio_ticks,
         guest_time, cguest_time, start_data, end_data,
         start_brk, arg_start, arg_end, env_start, env_end,
         exit_code;

  // useful fields
  int64_t utime, stime, vsize, rss;

  ifstream stat_stream("/proc/self/stat", ios_base::in);
  stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
              >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
              >> utime >> stime >> cutime >> cstime >> priority >> nice
              >> num_threads >> itrealvalue >> starttime >> vsize >> rss;
  stat_stream.close();

  const double&& page_size_mb = sysconf(_SC_PAGE_SIZE) / 1024.0 / 1024.0;

  // double vm_mb       = vsize / 1024.0 / 1024.0;
  res->resident_mb = rss * page_size_mb;
  res->user_time   = utime / sysconf(_SC_CLK_TCK);
  res->system_time  = stime / sysconf(_SC_CLK_TCK);

  return true;
}

bool get_system_resource_used(struct ResourceUsed *res) {
  if (nullptr == res) {
    return false;
  }

  return true;
}

#endif  // MODEL_SERVER_SRC_UTIL_OS_LINUX_RESOURCE_USED_H_
