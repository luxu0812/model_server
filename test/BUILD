cc_test(
  name = "test_tf_engine",
  srcs = ["engine/test_tf_engine.cpp"],
  deps = [
    "@//src:engine",
    "@com_google_googletest//:gtest",
    "@com_github_google_glog//:glog",
    "@com_github_gflags_gflags//:gflags",
  ],
  data = [
    "data/model1/graph.pb",
    "data/model1/graph_meta.json",
    "data/model1/samples.dat",
  ],
  timeout = "short",
)

cc_test(
  name = "test_onnx_engine",
  srcs = ["engine/test_onnx_engine.cpp"],
  deps = [
    "@//src:engine",
    "@com_google_googletest//:gtest",
    "@com_github_google_glog//:glog",
    "@com_github_gflags_gflags//:gflags",
  ],
  data = [
    "data/model1/graph.onnx",
    "data/model1/graph_meta.json",
    "data/model1/samples.dat",
  ],
  timeout = "short",
)