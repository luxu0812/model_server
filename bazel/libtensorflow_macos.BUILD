cc_library(
  name = "tensorflow",
  hdrs = glob([
    "include/**/*.h",
  ]),
  includes = [
    "include",
  ],
  srcs = ["lib/libtensorflow.dylib", "lib/libtensorflow_framework.dylib"],
  deps = [
    "@com_google_protobuf//:protobuf",
  ],
  visibility = ["//visibility:public"],
)
