DEFAULT_SETUP_DEPS=true
DEFAULT_SETUP_PYTHON=false
DEFAULT_SETUP_OS=false
DEFAULT_SETUP_ONNX_MKL=false
DEFAULT_SETUP_ONNX_DNNL=false
DEFAULT_SETUP_ONNX_OPENVINO=false

function get_shell_config() {
  shell_config=${HOME}/.bashrc
  if [[ ${SHELL} == "/bin/zsh" ]]; then
    shell_config=${HOME}/.zshrc
  fi
  echo ${shell_config}
}

function setup_os() {
  if ! [[ ${SETUP_OS} = true || ${DEFULAT_SETUP_OS} = true ]]; then
    echo "setup os skipped, use SETUP_OS=true to enable"
    return
  fi

  uname=`uname`
  if [[ "${uname}" == "Darwin" ]]; then
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    brew install autoconf	libfido2 m4 bazelisk libidn2 mpdecimal ca-certificates libmpc mpfr cairo libomp openjdk \
      coreutils libpng openjdk@11 fontconfig libpthread-stubs openssl@1.1 freetype libtiff openssl@3 gettext      \
      libunistring pcre2 giflib libx11 pixman git libxau pkg-config glib libxcb protobuf gmp libxdmcp readline    \
      graphite2 libxext snappy harfbuzz libxrender wget icu4c little-cms2 xorgproto isl lrzsz xz jpeg-turbo lz4   \
      zlib libcbor lzlib libevent lzo
  else
    apt-get update && apt-get install -y apt-transport-https build-essential libcurl4-openssl-dev libcppunit-dev   \
      libunwind-dev libevent-dev libsasl2-dev libzstd-dev libssl-dev libbz2-dev liblz4-dev zlib1g-dev binutils-dev \
      ant g++ gcc autoconf libtool automake pkg-config vim wget curl gnupg tree psmisc net-tools nethogs sysstat   \
      iputils-ping htop iotop iftop oprofile libgoogle-perftools-dev llvm clang git libsqlite3-dev liblzma-dev     \
      libreadline-dev libbz2-dev
  fi
}

function setup_python() {
  if ! [[ ${SETUP_PYTHON} = true || ${DEFAULT_SETUP_PYTHON} = true ]]; then
    echo "setup python skipped, use SETUP_PYTHON=true to enable"
    return
  fi

  if [ -d ~/.pyenv ]; then
    echo "pyenv already installed"
    return
  fi

  shell_config=$(get_shell_config)

  mkdir -p ${HOME}/.local/bin
  curl https://pyenv.run | bash
  echo '
  export PATH="${HOME}/.local/bin:${HOME}/.pyenv/bin:${PATH}"
  eval "$(pyenv init --path)"
  eval "$(pyenv init -)"' >> ${shell_config}
  source ${shell_config}
  pyenv install 3.11.0
  pyenv global 3.11.0
}

function setup_bazel() {
  if [[ -f ${HOME}/.local/bin/bazelisk ]]; then
    echo "bazelisk already installed"
    return
  fi

  uname=`uname`
  url="https://github.com/bazelbuild/bazelisk/releases/download/v1.17.0/bazelisk-linux-amd64"
  if [[ "${uname}" == "Darwin" ]]; then
    url="https://github.com/bazelbuild/bazelisk/releases/download/v1.17.0/bazelisk-darwin-arm64"
  fi

  mkdir -p ${HOME}/.local/bin
  wget ${url} -O ${HOME}/.local/bin/bazelisk
  chmod +x ${HOME}/.local/bin/bazelisk

  shell_config=$(get_shell_config)
  echo 'export PATH="${HOME}/.local/bin:${PATH}"' >> ${shell_config}
}

function setup_cmake() {
  if [[ -d ${HOME}/.local/cmake ]]; then
    echo "cmake already installed"
    return
  fi

  shell_config=$(get_shell_config)
  pushd ${HOME}/.local/build
  wget https://github.com/Kitware/CMake/releases/download/v3.27.0-rc4/cmake-3.27.0-rc4.tar.gz
  tar zxvf cmake-3.27.0-rc4.tar.gz
  rm cmake-3.27.0-rc4.tar.gz
  pushd cmake-3.27.0-rc4
  ./bootstrap --prefix=~/.local/cmake
  make -j10 && make install
  echo 'export PATH="${HOME}/.local/cmake/bin:${PATH}"' >> ${shell_config}
  source ${shell_config}
  popd
  popd
}

function setup_gflags() {
  if [[ -d ${HOME}/.local/lib/gflags ]]; then
    echo "gflags already installed"
    return
  fi

  pushd ${HOME}/.local/build
  git clone https://github.com/gflags/gflags.git
  pushd gflags
  git checkout tags/v2.2.2 -b v2.2.2
  mkdir build
  cmake -DCMAKE_INSTALL_PREFIX=~/.local/lib/gflags -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-fPIC" -S . -B build
  cmake --build build -j10
  cmake --build build --target install
  popd
}

function setup_glog() {
  if [[ -d ${HOME}/.local/lib/glog ]]; then
    echo "glog already installed"
    return
  fi

  pushd ${HOME}/.local/build
  git clone https://github.com/google/glog.git
  pushd glog
  git checkout tags/v-1.6.0 -b v0.6.0
  mkdir build
  cmake -DCMAKE_INSTALL_PREFIX=~/.local/lib/glog -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -S . -B build
  cmake --build build -j10
  cmake --build build --target install
  popd
  popd
}

function setup_googletest() {
  if [[ -d ${HOME}/.local/lib/googletest ]]; then
    echo "googletest already installed"
    return
  fi

  pushd ${HOME}/.local/build
  git clone https://github.com/google/googletest.git
  pushd googletest
  git checkout tags/v1.13.0 -b v1.13.0
  mkdir build
  cmake -DCMAKE_INSTALL_PREFIX=~/.local/lib/googletest -DCMAKE_BUILD_TYPE=Release -S . -B build
  cmake --build build -j10
  cmake --build build --target install
  popd
  popd
}

function setup_google_benchmark() {
  if [[ -d ${HOME}/.local/lib/benchmark ]]; then
    echo "google benchmark already installed"
    return
  fi

  pushd ${HOME}/.local/build
  git clone https://github.com/google/benchmark.git
  pushd benchmark
  git checkout tags/v1.8.2 -b v1.8.2
  mkdir build
  cmake -DCMAKE_INSTALL_PREFIX=~/.local/lib/benchmark -DCMAKE_BUILD_TYPE=Release -DGOOGLETEST_PATH=../googletest -S . -B build
  cmake --build build -j10
  cmake --build build --target install
  popd
  popd
}

function setup_tensorflow() {
  if [[ -d ${HOME}/.local/lib/libtensorflow ]]; then
    echo "tensorflow already installed"
    return
  fi

  pushd ${HOME}/.local/build
  git clone https://github.com/tensorflow/tensorflow.git
  pushd tensorflow
  git checkout tags/v2.13.0 -b v2.13.0
  ./configure
  bazelisk clean --expunge
  bazelisk build --jobs=10 --compilation_mode=opt --config=mkl --spawn_strategy=sandboxed tensorflow/tools/lib_package:libtensorflow
  mkdir -p ~/.local/lib/libtensorflow
  tar zxvf bazel-bin/tensorflow/tools/lib_package/libtensorflow.tar.gz -C ~/.local/lib/libtensorflow
  cp -r bazel-bin/tensorflow/core/protobuf ~/.local/lib/libtensorflow/include/tensorflow/core
  cp -r bazel-bin/tensorflow/core/framework ~/.local/lib/libtensorflow/include/tensorflow/core
  cp -r bazel-bin/tensorflow/tsl ~/.local/lib/libtensorflow/include/tensorflow
  popd
  popd
}

function setup_zlib() {
  if [[ -d ${HOME}/.local/lib/zlib ]]; then
    echo "zlib already installed"
    return
  fi

  pushd ${HOME}/.local/build
  cp -r tensorflow/bazel-tensorflow/external/zlib ./zlib
  pushd zlib
  ./configure --prefix=~/.local/lib/zlib
  make -j10 && make install
  popd
  popd
}

function setup_protobuf() {
  if [[ -d ${HOME}/.local/lib/protobuf ]]; then
    echo "protobuf already installed"
    return
  fi

  pushd ${HOME}/.local/build
  cp -r tensorflow/bazel-tensorflow/external/com_google_protobuf ./protobuf
  pushd protobuf
  mkdir build
  cmake -DCMAKE_INSTALL_PREFIX=~/.local/lib/protobuf -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release -S . -B build
  cmake --build build -j10
  cmake --build build --target install
  popd
  popd
}

function setup_abseil() {
  if [[ -d ${HOME}/.local/lib/absl ]]; then
    echo "absl already installed"
    return
  fi

  pushd ${HOME}/.local/build
  cp -r tensorflow/bazel-tensorflow/external/com_google_absl ./com_google_absl
  pushd com_google_absl
  mkdir build
  cmake -DCMAKE_INSTALL_PREFIX=~/.local/lib/absl -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=20 -S . -B build
  cmake --build build -j10
  cmake --build build --target install
  popd
  popd
}

function setup_onnx() {
  if [[ -d ${HOME}/.local/lib/onnxruntime ]]; then
    echo "onnxruntime already installed"
    return
  fi

  pushd ${HOME}/.local/build
  git clone https://github.com/microsoft/onnxruntime.git # or (https://github.com/intel/onnxruntime.git)
  pushd onnxruntime
  git checkout tags/v1.15.0 -b v1.15.0

  uname=`uname`
  if [[ "${uname}" == "Darwin" ]]; then
    ./build.sh --config Release --build_shared_lib --parallel                                                       \
      --cmake_extra_defines CMAKE_INSTALL_PREFIX:PATH=~/.local/lib/onnxruntime                                      \
      --cmake_extra_defines CMAKE_CXX_FLAGS="-Wno-error=unused-but-set-variable -Wno-error=deprecated-declarations" \
      --cmake_extra_defines CMAKE_C_FLAGS="-Wno-error=unused-but-set-variable -Wno-error=deprecated-declarations"
  else
    ./build.sh --config Release --build_shared_lib --parallel                                        \
      --cmake_extra_defines CMAKE_INSTALL_PREFIX:PATH=~/.local/lib/onnxruntime                       \
      --cmake_extra_defines CMAKE_CXX_FLAGS="-Wno-error=maybe-uninitialized -Wno-error=array-bounds" \
      --cmake_extra_defines CMAKE_C_FLAGS="-Wno-error=maybe-uninitialized -Wno-error=array-bounds"
  fi
  pushd build/Linux/Release && make install && popd
  popd
  popd
}

function setup_onnx_mkl() {
  if ! [[ ${SETUP_ONNX_MKL} = true || ${DEFAULT_SETUP_ONNX_MKL} = true ]]; then
    echo "setup onnxruntime_mkl skipped, use SETUP_ONNX_MKL=true to enable"
    return
  fi
  if [[ -d ${HOME}/.local/lib/onnxruntime-mkl ]]; then
    echo "onnxruntime-mkl already installed"
    return
  fi

  pushd ${HOME}/.local/build
  git clone https://github.com/microsoft/onnxruntime.git # or (https://github.com/intel/onnxruntime.git)
  pushd onnxruntime
  git checkout tags/v1.15.0 -b v1.15.0
  ./build.sh --config Release --build_shared_lib --parallel                                        \
    --cmake_extra_defines CMAKE_INSTALL_PREFIX:PATH=~/.local/lib/onnxruntime-mkl                   \
    --cmake_extra_defines CMAKE_CXX_FLAGS="-Wno-error=uninitialized -Wno-error=array-bounds -Wno-error=unused-variable -Wno-error=unknown-pragmas -Wno-error=unused-command-line-argument -DEIGEN_USE_MKL_ALL -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_sequential -lmkl_core -lstdc++ -lpthread -lm -lrt -ldl -lgomp" \
    --cmake_extra_defines CMAKE_C_FLAGS="-Wno-error=uninitialized -Wno-error=array-bounds -Wno-error=unused-variable -Wno-error=unknown-pragmas -Wno-error=unused-command-line-argument -DEIGEN_USE_MKL_ALL -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_sequential -lmkl_core -lstdc++ -lpthread -lm -lrt -ldl -lgomp"

  pushd build/Linux/Release && make install && popd
  popd
  popd
}

function setup_onnx_dnnl() {
  if ! [[ ${SETUP_ONNX_DNNL} = true && ${DEFAULT_SETUP_ONNX_DNNL} = true ]]; then
    echo "setup onnxruntime_dnnl skipped, use SETUP_ONNX_DNNL=true to enable"
    return
  fi
  if [[ -d ${HOME}/.local/lib/onnxruntime_dnnl ]]; then
    echo "onnxruntime_dnnl already installed"
    return
  fi

  pushd ${HOME}/.local/build
  git clone https://github.com/microsoft/onnxruntime.git # or (https://github.com/intel/onnxruntime.git)
  pushd onnxruntime
  git checkout tags/v1.15.0 -b v1.15.0
  ./build.sh --config Release --build_shared_lib --parallel --use_dnnl                             \
    --cmake_extra_defines CMAKE_INSTALL_PREFIX:PATH=~/.local/lib/onnxruntime_dnnl                  \
    --cmake_extra_defines CMAKE_CXX_FLAGS="-Wno-error=maybe-uninitialized -Wno-error=array-bounds" \
    --cmake_extra_defines CMAKE_C_FLAGS="-Wno-error=maybe-uninitialized -Wno-error=array-bounds"
  pushd build/Linux/Release && make install && popd
  popd
  popd
}

function setup_onnx_openvino() {
  if ! [[ ${SETUP_ONNX_OPENVINO} = true || ${DEFAULT_SETUP_ONNX_OPENVINO} = true ]]; then
    echo "setup onnxruntime_openvino skipped, use SETUP_ONNX_OPENVINO=true to enable"
    return
  fi
  if [[ -d ${HOME}/.local/lib/onnxruntime_openvino ]]; then
    echo "onnxruntime_openvino already installed"
    return
  fi

  pushd ${HOME}/.local/build
  git clone https://github.com/microsoft/onnxruntime.git # or (https://github.com/intel/onnxruntime.git)
  pushd onnxruntime
  git checkout tags/v1.15.0 -b v1.15.0
  ./build.sh --config Release --build_shared_lib --parallel --use_openvino CPU_FP32                \
    --cmake_extra_defines CMAKE_INSTALL_PREFIX:PATH=~/.local/lib/onnxruntime_openvino              \
    --cmake_extra_defines CMAKE_CXX_FLAGS="-Wno-error=maybe-uninitialized -Wno-error=array-bounds" \
    --cmake_extra_defines CMAKE_C_FLAGS="-Wno-error=maybe-uninitialized -Wno-error=array-bounds"
  pushd build/Linux/Release && make install && popd
  popd
  popd
}

function setup_nlohmann_json() {
  if [[ -d ${HOME}/.local/lib/nlohmann_json ]]; then
    echo "nlohmann_json already installed"
    return
  fi

  pushd ${HOME}/.local/build
  git clone https://github.com/nlohmann/json.git
  pushd json
  git checkout tags/v3.11.2 -b v3.11.2
  cmake -DCMAKE_INSTALL_PREFIX=~/.local/lib/nlohmann_json -DCMAKE_BUILD_TYPE=Release -S . -B build
  cmake --build build -j10
  cmake --build build --target install
  popd
  popd
}

function setup_bshoshany_thread_pool() {
  if [[ -d ${HOME}/.local/lib/bs_thread_pool ]]; then
    echo "bs_thread_pool already installed"
    return
  fi

  pushd ${HOME}/.local/build
  git clone https://github.com/bshoshany/thread-pool.git
  pushd thread-pool
  git checkout tags/v3.5.0 -b v3.5.0
  mkdir -p ~/.local/lib/bs_thread_pool/include/BShoshany
  mv include/* ~/.local/lib/bs_thread_pool/include/BShoshany
  popd
  popd
}

function setup_jemalloc() {
  if [[ -d ${HOME}/.local/lib/jemalloc ]]; then
    echo "jemalloc already installed"
    return
  fi

  pushd ${HOME}/.local/build
  git clone https://github.com/jemalloc/jemalloc.git
  pushd jemalloc
  git checkout tags/5.3.0 -b 5.3.0
  ./autogen.sh
  ./configure --prefix=${HOME}/.local/lib/jemalloc
  make -j10 && make install
  popd
  popd
}

function setup_tcmalloc() {
  if [[ -d ${HOME}/.local/lib/tcmalloc ]]; then
    echo "tcmalloc already installed"
    return
  fi

  pushd ${HOME}/.local/build
  git clone https://github.com/google/tcmalloc.git
  mv tcmalloc ~/.local/lib/tcmalloc
  popd
}

function setup_deps() {
  if ! [[ ${SETUP_DEPS} = true || ${DEFAULT_SETUP_DEPS} = true ]]; then
    echo "setup deps skipped, use SETUP_DEPS=true to enable"
    return
  fi

  mkdir -p ${HOME}/.local/build
  setup_os && setup_python && setup_bazel && setup_cmake && setup_gflags && setup_glog && setup_googletest &&\
  setup_google_benchmark && setup_tensorflow && setup_zlib && setup_protobuf && setup_abseil && setup_nlohmann_json &&\
  setup_bshoshany_thread_pool && setup_jemalloc && setup_tcmalloc && setup_onnx && setup_onnx_mkl &&\
  setup_onnx_dnnl && setup_onnx_openvino
  rm -rf ${HOME}/.local/build
}
