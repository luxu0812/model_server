cc_library(
  name = "tensorflow",
  hdrs = glob([
    "include/**/*.h",
  ]),
  includes = [
    "include",
  ],
  srcs = ["lib/libtensorflow.so"],
  deps = [
    "@com_google_absl//:absl",
    "@com_google_protobuf//:protobuf",
    "@eigen//:eigen"
  ],
  visibility = ["//visibility:public"],
)
