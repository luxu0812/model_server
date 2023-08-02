cc_library(
  name = "tensorflow",
  hdrs = glob([
    "include/**/*.h",
  ]),
  includes = [
    "include",
  ],
  srcs = ["lib/libtensorflow.so.2", "lib/libtensorflow_framework.so.2"],
  visibility = ["//visibility:public"],
)
