#!/bin/bash

#----------------------------- working directory -----------------------------#
REAL_FILE=$(readlink -f $0)
SCRIPT_NAME=${REAL_FILE##*/}
SCRIPT_DIR=$(cd "$(dirname "${REAL_FILE}")"; pwd)

pushd ${SCRIPT_DIR}
source ${SCRIPT_DIR}/script/functional.sh

#------------------------------ trap debug info ------------------------------#
trap 'error_info ${SCRIPT_NAME} ${LINENO} $?' ERR
# trap 'debug_info ${SCRIPT_NAME} ${LINENO}' DEBUG

#----------------------------------- clean -----------------------------------#
# bazel clean --expunge

#----------------------------------- setup -----------------------------------#
setup

#----------------------------------- test ------------------------------------#
if [[ "${STATIC_CODE_CHECK}" = true || "${DEFAULT_STATIC_CODE_CHECK}" = true ]]; then
  log ${SCRIPT_NAME} ${LINENO} "static analysis is running ..."
  static_code_check
else
  log ${SCRIPT_NAME} ${LINENO} "static analysis is omitted."
fi

if [[ "${UNIT_TEST}" = true || "${DEFAULT_UNIT_TEST}" = true ]]; then
  log ${SCRIPT_NAME} ${LINENO} "unit test is running ..."
  unit_test
else
  log ${SCRIPT_NAME} ${LINENO} "unit test is omitted."
fi

if [[ "${BENCHMARK_TEST}" = true || "$DEFAULT_BENCHMARK_TEST" = true ]]; then
  log ${SCRIPT_NAME} ${LINENO} "benchmark test is running ..."
  benchmark_test
else
  log ${SCRIPT_NAME} ${LINENO} "benchmark test is omitted."
fi

#----------------------------------- build -----------------------------------#
bazelisk build //src:perf_graph \
  --jobs=10                     \
  --cxxopt='-std=c++17'         \
  --compilation_mode opt
# ${SCRIPT_DIR}/bazel-bin/src/perf_graph
