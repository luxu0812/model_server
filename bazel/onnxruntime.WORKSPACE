# This file marks the root of the Bazel workspace.
# See MODULE.bazel for external dependencies setup.
workspace(name = "onnxruntime")

bazel_dep(name = "com_google_googletest")
local_path_override(
    module_name = "com_google_googletest",
    path = "/Users/xu.lu/.local/lib/googletest",
)

bazel_dep(name = "com_google_absl")
local_path_override(
    module_name = "com_google_absl",
    path = "/Users/xu.lu/.local/lib/absl",
)

bazel_dep(name = "protobuf")
local_path_override(
    module_name = "protobuf",
    path = "/Users/xu.lu/.local/lib/protobuf",
)

bazel_dep(name = "nlohmann_json")
local_path_override(
    module_name = "nlohmann_json",
    path = "/Users/xu.lu/.local/lib/nlohmann_json",
)
