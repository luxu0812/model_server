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
  name = "util",
  hdrs = [
    "util/functional/scope_exit_task.h",
    "util/functional/random.h",
    "util/functional/timer.h",
    "util/process/process_initiator.h"
  ],
  srcs = [
  ],
  deps = [
    "@com_github_google_glog//:glog",
    "@com_github_gflags_gflags//:gflags",
    "@com_google_absl//:absl",
  ],
  strip_include_prefix = "util",
  include_prefix = "infer_engine/src/util",
  copts = copts,
  visibility = ["//visibility:public"],
)

cc_library(
  name = "data",
  hdrs = [
    "data/return_status.h",
    "data/model_spec.h",
    "data/type.h"
  ],
  srcs = [
    "data/return_status.cpp"
  ],
  deps = [
    ":util",
    "@com_github_google_glog//:glog",
    "@com_google_absl//:absl",
    "@nlohmann_json//:nlohmann_json",
  ],
  strip_include_prefix = "data",
  include_prefix = "infer_engine/src/data",
  copts = copts,
  visibility = ["//visibility:public"],
)

cc_library(
  name = "engine",
  hdrs = [
    "engine/engine.h",
    "engine/tf_engine.h",
    "engine/onnx_engine.h",
  ],
  srcs = [
    "engine/tf_engine.cpp",
    "engine/onnx_engine.cpp",
  ],
  deps = [
    ":util",
    ":data",
    "@com_github_google_glog//:glog",
    "@com_google_absl//:absl",
    "@com_google_protobuf//:protobuf",
    "@tensorflow//:tensorflow",
    "@onnxruntime//:onnxruntime",
  ],
  strip_include_prefix = "engine",
  include_prefix = "infer_engine/src/engine",
  copts = copts,
  visibility = ["//visibility:public"],
)

cc_binary(
  name = "perf_graph",
  srcs = [
    "tool/perf_graph.cpp",
  ],
  deps = [
    ":util",
    ":data",
    ":engine",
    "@com_github_google_glog//:glog",
    "@com_github_gflags_gflags//:gflags",
    "@bs_thread_pool//:bs_thread_pool",
  ],
  # linkopts = ["-Wl,-rpath,$ORIGIN/../lib"],
)
