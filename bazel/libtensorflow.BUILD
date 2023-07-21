cc_library(
  name = "tensorflow",
  hdrs = glob([
    "include/**",
  ]),
  includes = [
    "include",
  ],
  srcs = ["lib/libtensorflow.so", "lib/libiomp5.so", "lib/libmklml_intel.so"],
  visibility = ["//visibility:public"],
)
