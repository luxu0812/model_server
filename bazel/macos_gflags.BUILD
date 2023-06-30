cc_library(
  name = "gflags_nothreads",
  hdrs = glob([
    "include/**/*.h",
  ]),
  includes = [
    "include",
  ],
  srcs = ["lib/libgflags_nothreads.dylib"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "gflags",
  hdrs = glob([
    "include/**/*.h",
  ]),
  includes = [
    "include",
  ],
  srcs = ["lib/libgflags.dylib"],
  linkopts = [
    "-lpthread",
  ],
  visibility = ["//visibility:public"],
)
