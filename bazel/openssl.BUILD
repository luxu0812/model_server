cc_library(
  name = "crypto",
  hdrs = glob([
    "include/**/*.h",
  ]),
  includes = [
    "include",
  ],
  srcs = ["lib/libcrypto.a"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "ssl",
  hdrs = glob([
    "include/**/*.h",
  ]),
  includes = [
    "include",
  ],
  srcs = ["lib/libssl.a"],
  deps = [
    ":crypto",
  ],
  linkopts = [
    "-ldl",
  ],
  visibility = ["//visibility:public"],
)