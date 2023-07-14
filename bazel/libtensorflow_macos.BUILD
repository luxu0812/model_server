cc_library(
  name = "tensorflow",
  hdrs = glob([
    "include/**/*.h",
  ]),
  includes = [
    "include",
  ],
  srcs = [
    "lib/libtensorflow.2.dylib",
    "lib/libtensorflow_framework.2.dylib"
  ],
  deps = [
    "@com_google_protobuf//:protobuf",
  ],
  visibility = ["//visibility:public"],
)
