cc_library(
  name = "tensorflow",
  hdrs = glob([
    "include/**/*.h",
  ]),
  includes = [
    "include",
  ],
  srcs = [
    "lib/libiomp5.so",
    "lib/libmklml_intel.so",
    "lib/libtensorflow.so",
  ],
  visibility = ["//visibility:public"],
)