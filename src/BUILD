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
  deps = [
    "@com_github_google_glog//:glog"
  ],
  strip_include_prefix = "data",
  include_prefix = "infer_engine/src/data",
  copts = copts,
  visibility = ["//visibility:public"],
)

cc_library(
  name = "inference_engine",
  hdrs = [
    "inference_engine/inference_engine.h",
    "inference_engine/xla_engine.h",
    "inference_engine/onnx_engine.h",
  ],
  srcs = [
    "inference_engine/xla_engine.cpp",
    "inference_engine/onnx_engine.cpp",
  ],
  deps = [
    ":data",
    "@tensorflow//:tensorflow",
    "@onnxruntime//:onnxruntime",
  ],
  strip_include_prefix = "inference_engine",
  include_prefix = "infer_engine/src/inference_engine",
  copts = copts,
  visibility = ["//visibility:public"],
)
