cc_library(
  name = "tensorflow",
  hdrs = glob([
    "include/**/*.h",
  ]),
  includes = [
    "include",
  ],
  # srcs = glob([
  #   "include/**/*.cc",
  #   "include/**/*.cpp",
  # ]) + ["lib/libtensorflow.dylib", "lib/libtensorflow_framework.dylib"],
  srcs = ["lib/libtensorflow.dylib", "lib/libtensorflow_framework.dylib"],
  visibility = ["//visibility:public"],
)
