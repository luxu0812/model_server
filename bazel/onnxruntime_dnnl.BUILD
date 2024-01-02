cc_library(
  name = "onnxruntime",
  hdrs = glob([
    "include/**",
  ]),
  includes = [
    "include",
  ],
  deps = [
    "@dnnl//:dnnl"
  ]
  srcs = [
    "lib/libonnxruntime_providers_dnnl.so",
    "lib/libonnxruntime_providers_shared.so",
    "lib/libonnxruntime.so.1.16.3",
    "lib/libonnxruntime.so",
  ],
  visibility = ["//visibility:public"],
)
