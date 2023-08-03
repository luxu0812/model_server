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
    "@com_google_absl//:absl",
  ],
  strip_include_prefix = "util",
  include_prefix = "model_server/src/util",
  copts = copts,
  visibility = ["//visibility:public"],
)

cc_library(
  name = "data",
  hdrs = [
    "data/return_status.h",
    "data/roster.h",
    "data/model_spec.h",
    "data/runtime_conf.h",
    "data/sample.h"
  ],
  srcs = [
    "data/return_status.cpp",
    "data/roster.cpp",
    "data/model_spec.cpp",
  ],
  deps = [
    ":util",
    "@com_github_google_glog//:glog",
    "@com_google_absl//:absl",
    "@nlohmann_json//:nlohmann_json",
  ],
  strip_include_prefix = "data",
  include_prefix = "model_server/src/data",
  copts = copts,
  visibility = ["//visibility:public"],
)

cc_library(
  name = "engine_base",
  hdrs = [
    "engine/engine.h",
  ],
  deps = [
    ":data"
  ],
  strip_include_prefix = "engine",
  include_prefix = "model_server/src/engine",
  copts = copts,
  visibility = ["//visibility:public"],
)

cc_library(
  name = "tf_engine",
  hdrs = [
    "engine/tf_engine.h",
  ],
  srcs = [
    "engine/tf_engine.cpp",
  ],
  deps = [
    ":util",
    ":data",
    ":engine_base",
    "@com_github_google_glog//:glog",
    "@com_google_absl//:absl",
    "@com_google_protobuf//:protobuf",
    "@tensorflow//:tensorflow",
  ],
  strip_include_prefix = "engine",
  include_prefix = "model_server/src/engine",
  copts = copts,
  visibility = ["//visibility:public"],
)

cc_library(
  name = "onnx_engine",
  hdrs = [
    "engine/onnx_engine.h",
  ],
  srcs = [
    "engine/onnx_engine.cpp",
  ],
  deps = [
    ":util",
    ":data",
    ":engine_base",
    "@com_github_google_glog//:glog",
    "@onnxruntime//:onnxruntime",
  ],
  strip_include_prefix = "engine",
  include_prefix = "model_server/src/engine",
  copts = copts,
  visibility = ["//visibility:public"],
)

cc_library(
  name = "lifecycle",
  hdrs = [
    "lifecycle/lifecycle.h",
  ],
  deps = [
    ":util",
    ":data",
    ":engine_base",
    "@com_github_google_glog//:glog",
    "@com_google_absl//:absl",
  ],
  strip_include_prefix = "lifecycle",
  include_prefix = "model_server/src/lifecycle",
  copts = copts,
  visibility = ["//visibility:public"],
)

cc_library(
  name = "population",
  hdrs = [
    "population/population.h",
  ],
  srcs = [
    "population/population.cpp",
  ],
  deps = [
    ":util",
    ":data",
    ":engine_base",
    ":lifecycle",
    "@com_github_google_glog//:glog",
    "@com_google_absl//:absl",
    "@bs_thread_pool//:bs_thread_pool",
  ],
  strip_include_prefix = "population",
  include_prefix = "model_server/src/population",
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
    ":tf_engine",
    ":onnx_engine",
    ":population",
    "@com_github_google_glog//:glog",
    "@com_github_gflags_gflags//:gflags",
    "@bs_thread_pool//:bs_thread_pool",
  ],
  # linkopts = ["-Wl,-rpath,$ORIGIN/../lib"],
)

cc_test(
  name = "test_tf_engine",
  srcs = ["unittest/engine/test_tf_engine.cpp"],
  deps = [
    ":util",
    ":engine_base",
    ":tf_engine",
    "@com_google_googletest//:gtest",
    "@com_github_google_glog//:glog",
    "@com_github_gflags_gflags//:gflags",
  ],
  data = [
    "@//data:model_1",
  ],
  malloc = select({
    ":use_tcmalloc": "@tcmalloc//:tcmalloc",
    ":use_jemalloc": "@jemalloc//:jemalloc",
    "//conditions:default": "@bazel_tools//tools/cpp:malloc",
  }),
  timeout = "short",
)

cc_test(
  name = "test_onnx_engine",
  srcs = ["unittest/engine/test_onnx_engine.cpp"],
  deps = [
    ":util",
    ":engine_base",
    ":onnx_engine",
    "@com_google_googletest//:gtest",
    "@com_github_google_glog//:glog",
    "@com_github_gflags_gflags//:gflags",
  ],
  data = [
    "@//data:model_1",
  ],
  malloc = select({
    ":use_tcmalloc": "@tcmalloc//:tcmalloc",
    ":use_jemalloc": "@jemalloc//:jemalloc",
    "//conditions:default": "@bazel_tools//tools/cpp:malloc",
  }),
  timeout = "short",
)
