cc_library(
  name = "onnxruntime",
  hdrs = glob([
    "include/**",
  ]),
  includes = [
    "include",
  ],
  srcs = [
    "libonnxruntime_providers_shared.so",
    "lib/libonnxruntime.so.1.16.3",
    "lib/libonnxruntime.so",
  ],
  visibility = ["//visibility:public"],
)
