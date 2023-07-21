cc_library(
  name = "tensorflow",
  hdrs = glob([
    "include/**",
  ]),
  includes = [
    "include",
  ],
  srcs = ["lib/libtensorflow.so", "libiomp5.so", "libmklml_intel.so"],
  visibility = ["//visibility:public"],
)
