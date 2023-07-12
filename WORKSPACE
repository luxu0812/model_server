# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

workspace(name = "InferEngine")

# ------------------------------  gflags  ------------------------------#
new_local_repository(
  name = "com_github_gflags_gflags",
  path = "/home/linuxbrew/.linuxbrew/Cellar/gflags",
  build_file = "bazel/gflags.BUILD",
)

# ------------------------------   glog   ------------------------------#
new_local_repository(
  name = "com_github_google_glog",
  path = "/home/linuxbrew/.linuxbrew/Cellar/glog",
  build_file = "bazel/glog.BUILD",
)

# ----------------------------  googletest  ----------------------------#
new_local_repository(
  name = "com_google_googletest",
  path = "/home/linuxbrew/.linuxbrew/Cellar/googletest",
  build_file = "bazel/googletest.BUILD",
)

# ------------------------------  abseil  ------------------------------#
new_local_repository(
  name = "com_google_absl",
  path = "/home/linuxbrew/.linuxbrew/Cellar/abseil",
  build_file = "bazel/absl.BUILD" 
)

# # ------------------------------  zlib  --------------------------------#
# new_local_repository(
#   name = "com_github_madler_zlib",
#   path = "/${HOME}/.conan/data/zlib/1.2.11/user/stable/package/d065d9b77a5f28035781daedf1fe27aa338ed934",
#   build_file = "bazel/zlib.BUILD"
# )
# 
# # -----------------------------  protobuf  -----------------------------#
# new_local_repository(
#   name = "com_google_protobuf",
#   path = "/${HOME}/.conan/data/protobuf/3.6.1/user/stable/package/8a0d45a6500f8b1a5c8dac95788a152d902fd44e",
#   build_file = "bazel/protobuf.BUILD"
# )
# 
# # ------------------------------ jemalloc ------------------------------#
# new_local_repository(
#   name = "jemalloc",
#   path = "/${HOME}/.conan/data/jemalloc/5.2.1-no-init-tls/user/stable/package/d065d9b77a5f28035781daedf1fe27aa338ed934",
#   build_file = "bazel/jemalloc.BUILD",
# )
# 
# # ------------------------------ openssl -------------------------------#
# new_local_repository(
#   name = "openssl",
#   path = "/${HOME}/.conan/data/openssl/1.0.2o/user/stable/package/8a0d45a6500f8b1a5c8dac95788a152d902fd44e",
#   build_file = "bazel/openssl.BUILD",
# )
# 
# # ------------------------------ leveldb -------------------------------#
# new_local_repository(
#   name = "leveldb",
#   path = "/${HOME}/.conan/data/leveldb/1.20/user/stable/package/d065d9b77a5f28035781daedf1fe27aa338ed934",
#   build_file = "bazel/leveldb.BUILD",
# )
# 
# # ----------------------------- tensorflow -----------------------------#
# new_local_repository(
#   name = "tensorflow",
#   path = "/root/.conan/data/tensorflow/1.15.5_pred/user/stable/package/7074939bced47b372f39e9ea6beadb82c7e0bc1c",
#   build_file = "bazel/tensorflow.BUILD",
# )
# 
# 