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
    "util/functional/return_status.h",
    "util/functional/scope_exit_task.h",
    "util/functional/random.h",
    "util/functional/timer.h",
    "util/process/process_initiator.h"
  ],
  srcs = [
    "util/functional/return_status.cpp",
  ],
  deps = [
    "@com_github_google_glog//:glog",
    "@com_google_absl//:absl",
  ],
  strip_include_prefix = "util",
  include_prefix = "model_server/src/util",
  visibility = ["//visibility:public"],
)

cc_library(
  name = "sample",
  hdrs = [
    "engine/sample.h"
  ],
  deps = [
  ],
  strip_include_prefix = "engine",
  include_prefix = "model_server/src/engine",
  visibility = ["//visibility:public"],
)

cc_library(
  name = "engine_base",
  hdrs = [
    "engine/engine.h",
  ],
  deps = [
    ":sample",
  ],
  strip_include_prefix = "engine",
  include_prefix = "model_server/src/engine",
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
    ":sample",
    ":engine_base",
    "@com_github_google_glog//:glog",
    "@com_google_absl//:absl",
    "@com_google_protobuf//:protobuf",
    "@tensorflow//:tensorflow",
  ],
  strip_include_prefix = "engine",
  include_prefix = "model_server/src/engine",
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
    ":sample",
    ":engine_base",
    "@com_github_google_glog//:glog",
    "@onnxruntime//:onnxruntime",
  ],
  strip_include_prefix = "engine",
  include_prefix = "model_server/src/engine",
  visibility = ["//visibility:public"],
)

cc_library(
  name = "population",
  hdrs = [
    "population/population.h",
    "population/lifecycle.h",
    "population/roster.h",
    "population/model_spec.h",
  ],
  srcs = [
    "population/population.cpp",
    "population/lifecycle.cpp",
    "population/roster.cpp",
    "population/model_spec.cpp",
  ],
  deps = [
    ":util",
    ":sample",
    ":engine_base",
    ":tf_engine",
    ":onnx_engine",
    "@com_github_google_glog//:glog",
    "@com_google_absl//:absl",
    "@bs_thread_pool//:bs_thread_pool",
    "@nlohmann_json//:nlohmann_json",
  ],
  strip_include_prefix = "population",
  include_prefix = "model_server/src/population",
  visibility = ["//visibility:public"],
)

cc_binary(
  name = "perf_graph",
  srcs = [
    "bin/perf_graph.cpp",
  ],
  deps = [
    ":util",
    ":sample",
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
    ":sample",
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
    ":sample",
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

cc_test(
  name = "bm_flat_hash_map",
  srcs = [
    "benchmark/bm_flat_hash_map.cpp",
  ],
  deps = [
    "@com_github_google_benchmark//:benchmark",
    "@com_google_absl//:absl",
  ],
  malloc = select({
    ":use_tcmalloc": "@tcmalloc//:tcmalloc",
    ":use_jemalloc": "@jemalloc//:jemalloc",
    "//conditions:default": "@bazel_tools//tools/cpp:malloc",
  }),
  timeout = "short",
)