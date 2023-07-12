cc_library(
  name = "glog",
  hdrs = glob([
    "include/**/*.h",
  ]),
  includes = [
    "include",
  ],
  srcs = ["lib/libglog.dylib"],
  deps = [
    "@com_github_gflags_gflags//:gflags",
  ],
  visibility = ["//visibility:public"],
)
