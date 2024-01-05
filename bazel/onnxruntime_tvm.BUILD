cc_library(
  name = "onnxruntime",
  hdrs = glob([
    "include/**",
  ]),
  includes = [
    "include",
    "include/onnxruntime/core/session",
  ],
  srcs = glob([
    # onnxruntime build without --build_shared_lib
    "lib/**/*.a",
    "lib/**/*.so",

    # onnxruntime build with --build_shared_lib
    # "lib/libonnxruntime_providers_shared.so",
    # "lib/libonnxruntime.so.1.16.3",
    # "lib/libonnxruntime.so",
  ]),
  linkopts = ["-ldl"],
  visibility = ["//visibility:public"],
)
