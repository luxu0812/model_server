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
      # onnxruntime build without --build_shared_lib
      "lib/libonnxruntime_common.a",
      "lib/libonnxruntime_flatbuffers.a",
      "lib/libonnxruntime_framework.a",
      "lib/libonnxruntime_graph.a",
      "lib/libonnxruntime_mlas.a",
      "lib/libonnxruntime_optimizer.a",
      "lib/libonnxruntime_providers.a",
      "lib/libonnxruntime_session.a",
      "lib/libonnxruntime_util.a",
      # onnxruntime build with --build_shared_lib
      # "lib/libonnxruntime_providers_shared.so",
      # "lib/libonnxruntime.so.1.16.3",
      # "lib/libonnxruntime.so",
    ]),
    "@bazel_tools//src/conditions:darwin": glob([
      # onnxruntime build without --build_shared_lib
      "lib/libonnxruntime_common.a",
      "lib/libonnxruntime_flatbuffers.a",
      "lib/libonnxruntime_framework.a",
      "lib/libonnxruntime_graph.a",
      "lib/libonnxruntime_mlas.a",
      "lib/libonnxruntime_optimizer.a",
      "lib/libonnxruntime_providers.a",
      "lib/libonnxruntime_session.a",
      "lib/libonnxruntime_util.a",
      # onnxruntime build with --build_shared_lib
      # "lib/libonnxruntime.1.16.3.dylib",
    ]),
    "//conditions:default": glob([
      # onnxruntime build without --build_shared_lib
      "lib/libonnxruntime_common.a",
      "lib/libonnxruntime_flatbuffers.a",
      "lib/libonnxruntime_framework.a",
      "lib/libonnxruntime_graph.a",
      "lib/libonnxruntime_mlas.a",
      "lib/libonnxruntime_optimizer.a",
      "lib/libonnxruntime_providers.a",
      "lib/libonnxruntime_session.a",
      "lib/libonnxruntime_util.a",
      # onnxruntime build with --build_shared_lib
      # "lib/libonnxruntime_providers_shared.so",
      # "lib/libonnxruntime.so.1.16.3",
      # "lib/libonnxruntime.so",
    ]),
  }),
  deps = [

  ],
  visibility = ["//visibility:public"],
)
