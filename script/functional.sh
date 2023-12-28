DEFAULT_CLEAN=false
DEFAULT_STATIC_CODE_CHECK=true
DEFAULT_UNIT_TEST=false
DEFAULT_BENCHMARK_TEST=false
DEFAULT_PERF_DEMO_GRAPH=false

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

function setup_bazel() {
  cp bazel/bazel_workspace ./WORKSPACE

  uname=`uname`
  if [[ "${uname}" == "Darwin" ]]; then
    sed -i "" "s|\${HOME}|${HOME}|g" WORKSPACE
  elif [[ "${uname}" == "Linux" ]]; then
    cp bazel/bazel_rc ./.bazelrc
    sed -i "s|\${HOME}|${HOME}|g" WORKSPACE
  else
    log ${SCRIPT_NAME} ${LINENO} "unknown operating system ${uname}"
    exit 1
  fi
}

function setup_bazel_module() {
  cp bazel/bazel_module ./MODULE.bazel

  uname=`uname`
  if [[ "${uname}" == "Darwin" ]]; then
    sed -i "" "s|\${HOME}|${HOME}|g" MODULE.bazel
  elif [[ "${uname}" == "Linux" ]]; then
    cp bazel/bazel_rc ./.bazelrc
    sed -i "s|\${HOME}|${HOME}|g" MODULE.bazel
  else
    log ${SCRIPT_NAME} ${LINENO} "unknown operating system ${uname}"
    exit 1
  fi

  if [[ -d "${HOME}/.local/lib/absl" ]]; then
    cp bazel/absl.WORKSPACE ${HOME}/.local/lib/absl/WORKSPACE
    cp bazel/absl.BUILD ${HOME}/.local/lib/absl/BUILD
    cp bazel/absl.MODULE ${HOME}/.local/lib/absl/MODULE.bazel
  fi
  if [[ -d "${HOME}/.local/lib/benchmark" ]]; then
    cp bazel/benchmark.WORKSPACE ${HOME}/.local/lib/benchmark/WORKSPACE
    cp bazel/benchmark.BUILD ${HOME}/.local/lib/benchmark/BUILD
    cp bazel/benchmark.MODULE ${HOME}/.local/lib/benchmark/MODULE.bazel
  fi
  if [[ -d "${HOME}/.local/lib/bs_thread_pool" ]]; then
    cp bazel/bs_thread_pool.WORKSPACE ${HOME}/.local/lib/bs_thread_pool/WORKSPACE
    cp bazel/bs_thread_pool.BUILD ${HOME}/.local/lib/bs_thread_pool/BUILD
    cp bazel/bs_thread_pool.MODULE ${HOME}/.local/lib/bs_thread_pool/MODULE.bazel
  fi
  if [[ -d "${HOME}/.local/lib/gflags" ]]; then
    cp bazel/gflags.WORKSPACE ${HOME}/.local/lib/gflags/WORKSPACE
    cp bazel/gflags.BUILD ${HOME}/.local/lib/gflags/BUILD
    cp bazel/gflags.MODULE ${HOME}/.local/lib/gflags/MODULE.bazel
  fi
  if [[ -d "${HOME}/.local/lib/glog" ]]; then
    cp bazel/glog.WORKSPACE ${HOME}/.local/lib/glog/WORKSPACE
    cp bazel/glog.BUILD ${HOME}/.local/lib/glog/BUILD
    cp bazel/glog.MODULE ${HOME}/.local/lib/glog/MODULE.bazel
  fi
  if [[ -d "${HOME}/.local/lib/googletest" ]]; then
    cp bazel/googletest.WORKSPACE ${HOME}/.local/lib/googletest/WORKSPACE
    cp bazel/googletest.BUILD ${HOME}/.local/lib/googletest/BUILD
    cp bazel/googletest.MODULE ${HOME}/.local/lib/googletest/MODULE.bazel
  fi
  if [[ -d "${HOME}/.local/lib/jemalloc" ]]; then
    cp bazel/jemalloc.WORKSPACE ${HOME}/.local/lib/jemalloc/WORKSPACE
    cp bazel/jemalloc.BUILD ${HOME}/.local/lib/jemalloc/BUILD
    cp bazel/jemalloc.MODULE ${HOME}/.local/lib/jemalloc/MODULE.bazel
  fi
  if [[ -d "${HOME}/.local/lib/libtensorflow" ]]; then
    cp bazel/libtensorflow.WORKSPACE ${HOME}/.local/lib/libtensorflow/WORKSPACE
    cp bazel/libtensorflow.BUILD ${HOME}/.local/lib/libtensorflow/BUILD
    cp bazel/libtensorflow.MODULE ${HOME}/.local/lib/libtensorflow/MODULE.bazel
  fi
  if [[ -d "${HOME}/.local/lib/nlohmann_json" ]]; then
    cp bazel/nlohmann_json.WORKSPACE ${HOME}/.local/lib/nlohmann_json/WORKSPACE
    cp bazel/nlohmann_json.BUILD ${HOME}/.local/lib/nlohmann_json/BUILD
    cp bazel/nlohmann_json.MODULE ${HOME}/.local/lib/nlohmann_json/MODULE.bazel
  fi
  if [[ -d "${HOME}/.local/lib/onnxruntime_dnnl" ]]; then
    cp bazel/onnxruntime_dnnl.WORKSPACE ${HOME}/.local/lib/onnxruntime_dnnl/WORKSPACE
    cp bazel/onnxruntime_dnnl.BUILD ${HOME}/.local/lib/onnxruntime_dnnl/BUILD
    cp bazel/onnxruntime_dnnl.MODULE ${HOME}/.local/lib/onnxruntime_dnnl/MODULE.bazel
  fi
  if [[ -d "${HOME}/.local/lib/onnxruntime" ]]; then
    cp bazel/onnxruntime.WORKSPACE ${HOME}/.local/lib/onnxruntime/WORKSPACE
    cp bazel/onnxruntime.BUILD ${HOME}/.local/lib/onnxruntime/BUILD
    cp bazel/onnxruntime.MODULE ${HOME}/.local/lib/onnxruntime/MODULE.bazel
  fi
  if [[ -d "${HOME}/.local/lib/protobuf" ]]; then
    cp bazel/protobuf.WORKSPACE ${HOME}/.local/lib/protobuf/WORKSPACE
    cp bazel/protobuf.BUILD ${HOME}/.local/lib/protobuf/BUILD
    cp bazel/protobuf.MODULE ${HOME}/.local/lib/protobuf/MODULE.bazel
  fi
  if [[ -d "${HOME}/.local/lib/zlib" ]]; then
    cp bazel/zlib.WORKSPACE ${HOME}/.local/lib/zlib/WORKSPACE
    cp bazel/zlib.BUILD ${HOME}/.local/lib/zlib/BUILD
    cp bazel/zlib.MODULE ${HOME}/.local/lib/zlib/MODULE.bazel
  fi
}

function setup() {
  bazel_version=$(cat .bazelversion)

  # Extract major version
  major_version=$(echo "$bazel_version" | cut -d. -f1)

  # Check if major version is smaller than or equal to 6
  if [[ $major_version -le 6 ]]; then
    setup_bazel
  else
    setup_bazel_module
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

function bazel_build() {
  bazelisk build                       \
    --jobs=10                          \
    --compilation_mode opt             \
    --cxxopt='-std=c++2a'              \
    --cxxopt='-Wno-unused-parameter'   \
    --cxxopt='-fno-omit-frame-pointer' \
    --cxxopt='-fPIC'                   \
  "$@"
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
    --cxxopt='-std=c++2a'              \
    --cxxopt='-Wno-unused-parameter'   \
    --cxxopt='-fno-omit-frame-pointer' \
    --cxxopt='-fPIC'                   \
    "$@"
  if [[ $? -ne 0 ]]; then
    return 1
  fi
}

function unit_test() {
  bazel_test //src:test_util --define "malloc=jemalloc" 
  if [[ $? -ne 0 ]]; then
    return 1
  fi
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

  bazel_test //src:bm_onnx_engine --define "malloc=jemalloc" --test_arg="--benchmark_format=console"
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
    log ${SCRIPT_NAME} ${LINENO} "static analysis is omitted, use STATIC_CODE_CHECK=ture to enable it."
  fi
  
  if [[ "${UNIT_TEST}" = true || "${DEFAULT_UNIT_TEST}" = true ]]; then
    log ${SCRIPT_NAME} ${LINENO} "unit test is running ..."
    unit_test
    if [[ $? -ne 0 ]]; then
      log ${SCRIPT_NAME} ${LINENO} "unit test failed."
      return 1
    fi
  else
    log ${SCRIPT_NAME} ${LINENO} "unit test is omitted, use UNIT_TEST=true to enable it."
  fi
  
  if [[ "${BENCHMARK_TEST}" = true || "${DEFAULT_BENCHMARK_TEST}" = true ]]; then
    log ${SCRIPT_NAME} ${LINENO} "benchmark test is running ..."
    benchmark_test
    if [[ $? -ne 0 ]]; then
      log ${SCRIPT_NAME} ${LINENO} "benchmark test failed."
      return 1
    fi
  else
    log ${SCRIPT_NAME} ${LINENO} "benchmark test is omitted, use BENCHMARK_TEST=true to enable it."
  fi
}

function clean() {
  if [[ "${CLEAN}" = true || "${DEFAULT_CLEAN}" = true ]]; then
    log ${SCRIPT_NAME} ${LINENO} "bazel cleaning ..."
    bazelisk clean --expunge
  else
    log ${SCRIPT_NAME} ${LINENO} "bazel cleaning is omitted, use CLEAN=true to enable it."
  fi
}

function perf_demo_graph() {
  if [[ "${PERF_DEMO_GRAPH}" = true || "${DEFAULT_PERF_DEMO_GRAPH}" = true ]]; then
    log ${SCRIPT_NAME} ${LINENO} "perf demo graph is running ..."
    engine_brands=("tf" "onnx")

    for engine_brand in ${engine_brands[@]}; do
      bazel_build //src:perf_${engine_brand} --define "malloc=jemalloc"
      if [[ $? -ne 0 ]]; then
        return 1
      fi
    done

    uname=`uname`
    if [[ "${uname}" == "Darwin" ]]; then
      for engine_brand in ${engine_brands[@]}; do
        ${SCRIPT_DIR}/bazel-bin/src/perf_${engine_brand} \
          --test_data_size=1000                          \
          --concurrency=1                                \
          --opt_level=1                                  \
          --jit_level=2                                  \
          --inter_op_parallelism_threads=6               \
          --intra_op_parallelism_threads=6
      done
    else
      for engine_brand in ${engine_brands[@]}; do
        numactl --cpunodebind=0 --membind=0                \
          ${SCRIPT_DIR}/bazel-bin/src/perf_${engine_brand} \
          --test_data_size=1000                            \
          --concurrency=1                                  \
          --opt_level=1                                    \
          --jit_level=2                                    \
          --inter_op_parallelism_threads=32                \
          --intra_op_parallelism_threads=20
      done
    fi
  else
    log ${SCRIPT_NAME} ${LINENO} "perf demo graph is omitted, use PERF_DEMO_GRAPH=true to enable it."
  fi
}
