# This file marks the root of the Bazel workspace.
# See MODULE.bazel for external dependencies setup.
workspace(name = "tensorflow")

#------------------------------- eigen --------------------------------#
new_local_repository(
  name = "eigen",
  path = "${HOME}/.local/lib/eigen",
  build_file = "bazel/BUILD",
)
