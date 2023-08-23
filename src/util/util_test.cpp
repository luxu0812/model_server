// Copyright 2021 zh.luxu1986@gmail.com

#include <string>
#include "gtest/gtest.h"
#include "glog/logging.h"
#include "absl/time/clock.h"
#include "util/os/thread_cpu.h"
#include "util/os/vpopen.h"
#include "util/os/network.h"
#include "util/os/resource_used.h"
#include "util/os/semaphore.h"
#include "util/io.h"
#include "util/comm.h"
#include "util/process/status.h"
#include "util/process/init.h"

using std::string;

/*
 * Ignore tests as they always fail on MACOS.
 *
 */
// TEST(UTIL_OS_THREAD_CPU, GET_THREAD_CPU_INIT_VALUE) {
//   int64_t get_cpu_id = -1;
//   ASSERT_TRUE(get_thread_cpu(&get_cpu_id));
//   ASSERT_EQ(get_cpu_id, 0);
// }
//
// TEST(UTIL_OS_THREAD_CPU, SET_THREAD_CPU) {
//   int64_t set_cpu_id = 5;
//   ASSERT_TRUE(set_thread_cpu(set_cpu_id));
//
//   int64_t get_cpu_id = -1;
//   ASSERT_TRUE(get_thread_cpu(&get_cpu_id));
//   ASSERT_EQ(get_cpu_id, set_cpu_id);
// }
//
// TEST(UTIL_OS_NETWORK, GET_HOST_IP) {
//   string ip;
//   ASSERT_TRUE(get_host_ip(&ip));
//   LOG(INFO) << "IP: " << ip;
// }

TEST(UTIL_OS_RESOURCE_USED, PROCESS) {
  struct ResourceUsed used;
  ASSERT_TRUE(get_process_resource_used(&used));
  LOG(INFO) << "resident_mb: " << used.resident_mb
            << ", user_time: "   << used.user_time
            << ", system_time: " << used.system_time;
}

TEST(UTIL_OS_RESOURCE_USED, SYSTEM) {
  struct ResourceUsed used;
  ASSERT_TRUE(get_system_resource_used(&used));
  LOG(INFO) << "resident_mb: " << used.resident_mb
            << ", user_time: "   << used.user_time
            << ", system_time: " << used.system_time;
}

TEST(UTIL_OS_VPOPEN, VPOPEN) {
  FILE *fp = vpopen("pwd", "r");
  ASSERT_NE(fp, nullptr);
  ASSERT_NE(vpclose(fp), -1);
}

TEST(UTIL_IO, READ_LINE) {
  FILE *fp = fopen("src/util/data/hello", "r");
  ASSERT_NE(fp, nullptr);

  string line;
  ASSERT_TRUE(WORKSPACE::util::io::read_line(fp, &line));
  ASSERT_STREQ(line.c_str(), "Hello world!");

  ASSERT_EQ(fclose(fp), 0);
}

TEST(UTIL_IO, READ_FILE) {
  FILE *fp = fopen("src/util/data/hello", "r");
  ASSERT_NE(fp, nullptr);

  string file;
  ASSERT_TRUE(WORKSPACE::util::io::read_file(fp, &file));
  ASSERT_STREQ(file.c_str(), "Hello world!\nAnd c++!\nbazel!\n");

  ASSERT_EQ(fclose(fp), 0);
}

TEST(UTIL_COMM, EXECUTE_VFORK) {
  ASSERT_TRUE(WORKSPACE::util::comm::execute_vfork("echo \"hello world\""));

  string out;
  ASSERT_TRUE(WORKSPACE::util::comm::execute_vfork("echo \"hello world\"", &out));
  ASSERT_STREQ(out.c_str(), "hello world\n");
}

TEST(UTIL_COMM, EXECUTE_SPAWN) {
  ASSERT_TRUE(WORKSPACE::util::comm::execute("echo \"hello world\""));

  string out;
  ASSERT_TRUE(WORKSPACE::util::comm::execute("echo \"hello world\"", &out));
  ASSERT_STREQ(out.c_str(), "hello world\n");
}

TEST(UTIL_PROCESS, STATUS) {
  WORKSPACE::util::process::Status status;
  absl::SleepFor(absl::Seconds(10));
}

TEST(UTIL_FUNCTIONAL, SEMAPHORE) {
  Semaphore s(5);
  s.wait();
  s.post();
}

int32_t main(int32_t argc, char *argv[]) {
  WORKSPACE::util::process::init(argc, argv);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
