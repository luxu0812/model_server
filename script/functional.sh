DEFAULT_CLEAN=false
DEFAULT_STATIC_CODE_CHECK=true
DEFAULT_UNIT_TEST=false
DEFAULT_BENCHMARK_TEST=false

function log() {
  echo "$(date +"%Y/%m/%d %H:%M:%S")][$1:$2][INFO] $3"
}

function debug_info() {
  cmd=`sed -n $2p $1`
  echo "$(date +"%Y/%m/%d %H:%M:%S")][$1:$2][DEBUG] execute command \"${cmd}\" ..."
}

function error_info() {
  let begin=$2-10
  let end=$2
  cmd=`sed -n ${begin},${end}p $1`
  echo "$(date +"%Y/%m/%d %H:%M:%S")][$1:$2][ERROR] exit with status $3, cmd:"
  echo "${cmd}"
  exit $3
}

function setup() {
  uname=`uname`
  if [[ "${uname}" == "Darwin" ]]; then
    cp bazel/WORKSPACE_MACOS ./WORKSPACE
    HOME_PATH=$(echo ~)
    sed -i "" "s|\${HOME}|${HOME}|g" WORKSPACE
  elif [[ "${uname}" == "Linux" ]]; then
    cp bazel/bazelrc_linux ./.bazelrc
    cp bazel/WORKSPACE_LINUX ./WORKSPACE
    HOME_PATH=$(echo ~)
    sed -i "s|\${HOME}|${HOME}|g" WORKSPACE
  else
    log ${SCRIPT_NAME} ${LINENO} "unknown operating system ${uname}"
    exit 1
  fi
}

function glob() {
  path=$1
  regex=$2

  for file in ${path}/* ; do
    if [[ -d "${file}" ]]; then
      glob ${file} ${regex}
    elif [[ -f "${file}" ]]; then
      if [[ "${file}" =~ ${regex} ]]; then
        echo ${file}
      fi
    fi
  done
}

function static_code_check() {
  srcs=`glob ./src ".*\.(c|cc|cpp|cxx|c\+\+|C|h|hh|hpp|hxx|inc)$"`
  cpplint.py --verbose=0         \
             --linelength=120    \
             --counting=detailed \
             --repository=..     \
             ${srcs}
  if [[ $? -ne 0 ]]; then
    return 1
  fi
}

function bazel_test() {
  bazelisk test                        \
    --compilation_mode opt             \
    --jobs=10                          \
    --test_output=all                  \
    --verbose_failures                 \
    --sandbox_debug                    \
    --test_verbose_timeout_warnings    \
    --dynamic_mode=off                 \
    --spawn_strategy=standalone        \
    --strategy=Genrule=standalone      \
    --cxxopt='-std=c++17'              \
    --cxxopt='-Wno-unused-parameter'   \
    --cxxopt='-fno-omit-frame-pointer' \
    --cxxopt='-fPIC'                   \
    "$@"
  if [[ $? -ne 0 ]]; then
    return 1
  fi
}

function unit_test() {
  bazel_test //src:test_tf_engine   --define "malloc=jemalloc"
  if [[ $? -ne 0 ]]; then
    return 1
  fi
  bazel_test //src:test_onnx_engine --define "malloc=jemalloc" 
  if [[ $? -ne 0 ]]; then
    return 1
  fi
}

function benchmark_test() {
  bazel_test //src:bm_flat_hash_map --define "malloc=jemalloc"
  if [[ $? -ne 0 ]]; then
    return 1
  fi

  bazel_test //src:bm_tf_engine --define "malloc=jemalloc" --test_arg="--benchmark_format=console"
  if [[ $? -ne 0 ]]; then
    return 1
  fi
}

function check() {
  if [[ "${STATIC_CODE_CHECK}" = true || "${DEFAULT_STATIC_CODE_CHECK}" = true ]]; then
    log ${SCRIPT_NAME} ${LINENO} "static analysis is running ..."
    static_code_check
    if [[ $? -ne 0 ]]; then
      log ${SCRIPT_NAME} ${LINENO} "static analysis failed."
      return 1
    fi
  else
    log ${SCRIPT_NAME} ${LINENO} "static analysis is omitted."
  fi
  
  if [[ "${UNIT_TEST}" = true || "${DEFAULT_UNIT_TEST}" = true ]]; then
    log ${SCRIPT_NAME} ${LINENO} "unit test is running ..."
    unit_test
    if [[ $? -ne 0 ]]; then
      log ${SCRIPT_NAME} ${LINENO} "unit test failed."
      return 1
    fi
  else
    log ${SCRIPT_NAME} ${LINENO} "unit test is omitted."
  fi
  
  if [[ "${BENCHMARK_TEST}" = true || "${DEFAULT_BENCHMARK_TEST}" = true ]]; then
    log ${SCRIPT_NAME} ${LINENO} "benchmark test is running ..."
    benchmark_test
    if [[ $? -ne 0 ]]; then
      log ${SCRIPT_NAME} ${LINENO} "benchmark test failed."
      return 1
    fi
  else
    log ${SCRIPT_NAME} ${LINENO} "benchmark test is omitted."
  fi
}

function clean() {
  if [[ "${CLEAN}" = true || "${DEFAULT_CLEAN}" = true ]]; then
    log ${SCRIPT_NAME} ${LINENO} "bazel cleaning ..."
    bazelisk clean --expunge
  else
    log ${SCRIPT_NAME} ${LINENO} "bazel cleaning is omitted."
  fi
}
