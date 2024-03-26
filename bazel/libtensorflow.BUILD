cc_library(
  name = "tensorflow",
  hdrs = glob([
    "include/**/*",
  ]),
  includes = [
    "include",
  ],
  srcs = select({
    "@bazel_tools//src/conditions:darwin_x86_64": glob([
      "lib/libtensorflow.so.2",
      "lib/libtensorflow_framework.so.2",
    ]),
    "@bazel_tools//src/conditions:darwin": glob([
      "lib/libtensorflow.2.dylib",
      "lib/libtensorflow_framework.2.dylib",
    ]),
    "//conditions:default": glob([
      "lib/libtensorflow.so.2",
      "lib/libtensorflow_framework.so.2",
    ]),
  }),
  visibility = ["//visibility:public"],
)

cc_library(
  name = "tensorflow_cc",
  hdrs = glob([
    "include/**/*",
  ]),
  includes = [
    "include",
  ],
  srcs = select({
    "@bazel_tools//src/conditions:darwin_x86_64": glob([
      "libcc/libtensorflow_cc.so.2",
      # "libcc/libtensorflow_framework.so.2",
    ]),
    "@bazel_tools//src/conditions:darwin": glob([
      "libcc/libtensorflow_cc.2.dylib",
      "libcc/libtensorflow_framework.2.dylib",
    ]),
    "//conditions:default": glob([
      "libcc/libtensorflow_cc.so.2",
      # "libcc/libtensorflow_framework.so.2",
    ]),
  }),
  visibility = ["//visibility:public"],
)
