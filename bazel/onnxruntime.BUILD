cc_library(
  name = "onnxruntime",
  hdrs = glob([
    "include/**",
  ]),
  includes = [
    "include",
  ],
  srcs = select({
    "@bazel_tools//src/conditions:darwin_x86_64": glob([
      "lib/libonnxruntime_providers_shared.so",
      "lib/libonnxruntime.so.1.16.3",
      "lib/libonnxruntime.so",
    ]),
    "@bazel_tools//src/conditions:darwin": glob([
      "lib/libonnxruntime.1.16.3.dylib",
    ]),
    "//conditions:default": glob([
      "lib/libonnxruntime_providers_shared.so",
      "lib/libonnxruntime.so.1.16.3",
      "lib/libonnxruntime.so",
    ]),
  }),
  visibility = ["//visibility:public"],
)
