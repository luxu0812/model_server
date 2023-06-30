copts = [
  "-Wno-unused-parameter",
  "-fno-omit-frame-pointer",
  "-fPIC",
]

config_setting(
  name = "use_jemalloc",
  values = {"define": "malloc=jemalloc"},
)

config_setting(
  name = "use_tcmalloc",
  values = {"define": "malloc=tcmalloc"},
)

config_setting(
  name = "use_mkl",
  values = {"define": "blas=mkl"},
)

cc_library(
  name = "data",
  hdrs = [
    "data/return_status.h",
  ],
  srcs = [
    "data/return_status.cpp"
  ],
  strip_include_prefix = "data",
  include_prefix = "infer_engine/src/data",
  copts = copts,
  visibility = ["//visibility:public"],
)
