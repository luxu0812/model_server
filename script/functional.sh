STATIC_CODE_CHECK=true
UNIT_TEST=false
BENCHMARK_TEST=false

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
      if [[ `basename ${file}` != "practice" ]]; then
        glob ${file} ${regex}
      fi
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
}

function unit_test() {
  bazelisk test //src:test_tf_engine   \
    --compilation_mode opt             \
    --jobs=10                          \
    --cxxopt='-std=c++17'              \
    --define "malloc=jemalloc"         \
    --test_output=all                  \
    --verbose_failures                 \
    --sandbox_debug                    \
    --test_verbose_timeout_warnings    \
    --dynamic_mode=off                 \
    --spawn_strategy=standalone        \
    --strategy=Genrule=standalone
  
  bazelisk test //src:test_onnx_engine \
    --compilation_mode opt             \
    --jobs=10                          \
    --cxxopt='-std=c++17'              \
    --define "malloc=jemalloc"         \
    --test_output=all                  \
    --verbose_failures                 \
    --sandbox_debug                    \
    --test_verbose_timeout_warnings    \
    --dynamic_mode=off                 \
    --spawn_strategy=standalone        \
    --strategy=Genrule=standalone
}

function benchmark_test() {
  bazelisk test //src:bm_flat_hash_map \
    --compilation_mode opt             \
    --jobs=10                          \
    --cxxopt='-std=c++17'              \
    --define "malloc=jemalloc"         \
    --test_output=all                  \
    --verbose_failures                 \
    --sandbox_debug                    \
    --test_verbose_timeout_warnings    \
    --dynamic_mode=off                 \
    --spawn_strategy=standalone        \
    --strategy=Genrule=standalone
}
