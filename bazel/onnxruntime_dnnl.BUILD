cc_library(
  name = "onnxruntime",
  hdrs = glob([
    "include/**",
  ]),
  includes = [
    "include",
  ],
  srcs = [
    "lib/libonnxruntime_providers_dnnl.so",
    "lib/libonnxruntime_providers_shared.so",
    "lib/libonnxruntime.so",
  ],
  visibility = ["//visibility:public"],
)
