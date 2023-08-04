#!/bin/bash

#----------------------------- working directory -----------------------------#
REAL_FILE=$(readlink -f $0)
SCRIPT_NAME=${REAL_FILE##*/}
SCRIPT_DIR=$(cd "$(dirname "${REAL_FILE}")"; pwd)

pushd ${SCRIPT_DIR}
source ${SCRIPT_DIR}/script/functional.sh

#------------------------------ trap debug info ------------------------------#
trap 'error_info ${SCRIPT_NAME} ${LINENO} $?' ERR

#----------------------------------- build -----------------------------------#
setup
clean
check

bazelisk build //src:perf_graph      \
  --jobs=10                          \
  --compilation_mode opt             \
  --cxxopt='-std=c++17'              \
  --cxxopt='-Wno-unused-parameter'   \
  --cxxopt='-fno-omit-frame-pointer' \
  --cxxopt='-fPIC'
${SCRIPT_DIR}/bazel-bin/src/perf_graph
