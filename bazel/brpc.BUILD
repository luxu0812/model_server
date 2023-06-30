cc_library(
  name = "brpc",
  hdrs = glob([
    "include/*.h",
    "include/**/*.h",
    "include/**/*.hpp",
  ]),
  includes = [
    "include",
  ],
  srcs = ["lib/libbrpc.a"],
  deps = [
    "@com_github_gflags_gflags//:gflags",
    "@com_google_protobuf//:protobuf",
    "@leveldb//:leveldb",
    "@openssl//:crypto",
    "@openssl//:ssl",
  ],
  linkopts = [
    # "-lssl -lcrypto -lzstd -lsasl2 -llz4",
  ],
  visibility = ["//visibility:public"],
)
