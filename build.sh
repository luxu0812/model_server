#!/bin/bash

#----------------------------- working directory -----------------------------#
REAL_FILE=$(readlink -f $0)
SCRIPT_NAME=${REAL_FILE##*/}
SCRIPT_DIR=$(cd "$(dirname "${REAL_FILE}")"; pwd)

pushd ${SCRIPT_DIR}
source ${SCRIPT_DIR}/script/functional.sh
source ${SCRIPT_DIR}/script/deps.sh

#------------------------------ trap debug info ------------------------------#
trap 'error_info ${SCRIPT_NAME} ${LINENO} $?' ERR

#----------------------------------- build -----------------------------------#
function setup_deps() {
  mkdir -p ${HOME}/.local/build
  setup_os
  setup_python
  setup_bazel
  setup_cmake
  setup_gflags
  setup_glog
  setup_googletest
  setup_google_benchmark
  setup_tensorflow
  setup_zlib
  setup_protobuf
  setup_abseil
  setup_nlohmann_json
  setup_bshoshany_thread_pool
  setup_jemalloc
  setup_tcmalloc
  setup_onnx
  setup_onnx_mkl
  setup_onnx_dnnl
  setup_onnx_openvino
}

setup_deps
setup
clean
check
perf_demo_graph
