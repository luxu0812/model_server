DEFAULT_SETUP_DEPS=true
DEFAULT_SETUP_PYTHON=false
DEFAULT_SETUP_OS=false
DEFAULT_SETUP_ONNX_MKL=false
DEFAULT_SETUP_ONNX_DNNL=false
DEFAULT_SETUP_ONNX_OPENVINO=false
DEFAULT_SETUP_GPU=false

function get_shell_config() {
  shell_name=$(basename "${SHELL}")
  shell_config=${HOME}/.bashrc
  if [[ ${shell_name} == "zsh" ]]; then
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
    apt-get update
    apt-get install -y apt-transport-https build-essential libcurl4-openssl-dev libcppunit-dev   \
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
  if [[ $? -ne 0 ]]; then
    echo "build cmake failed"
    exit 1
  fi
  echo 'export PATH="${HOME}/.local/cmake/bin:${PATH}"' >> ${shell_config}
  source ${shell_config}
  popd
  popd
}

function setup_cpplint() {
  if [[ -f ${HOME}/.local/bin/cpplint.py ]]; then
    echo "cpplint already installed"
    return
  fi

  mkdir -p ${HOME}/.local/bin
  wget https://raw.githubusercontent.com/cpplint/cpplint/master/cpplint.py -O ${HOME}/.local/bin/cpplint.py
  chmod +x ${HOME}/.local/bin/cpplint.py
}

function setup_skylib() {
  if [[ -d ${HOME}/.local/lib/bazel-skylib ]]; then
    echo "bazel-skylib already installed"
    return
  fi

  pushd ${HOME}/.local/build
  rm -rf bazel-skylib
  git clone https://github.com/bazelbuild/bazel-skylib.git
  pushd bazel-skylib
  git checkout tags/1.4.2 -b 1.4.2
  popd
  mv bazel-skylib ~/.local/lib
  popd
}

function setup_rules_pkg() {
  if [[ -d ${HOME}/.local/lib/rules_pkg ]]; then
    echo "rules_pkg already installed"
    return
  fi

  pushd ${HOME}/.local/build
  rm -rf rules_pkg
  git clone https://github.com/bazelbuild/rules_pkg.git
  pushd rules_pkg
  git checkout tags/0.9.1 -b 0.9.1
  popd
  mv rules_pkg ~/.local/lib
  popd
}

function setup_rules_foreign_cc() {
  if [[ -d ${HOME}/.local/lib/rules_foreign_cc ]]; then
    echo "rules_foreign_cc already installed"
    return
  fi

  pushd ${HOME}/.local/build
  rm -rf rules_foreign_cc
  git clone https://github.com/bazelbuild/rules_foreign_cc.git
  pushd rules_foreign_cc
  git checkout tags/0.9.0 -b 0.9.0
  popd
  mv rules_foreign_cc ~/.local/lib
  popd
}

function setup_rules_perl() {
  if [[ -d ${HOME}/.local/lib/rules_perl ]]; then
    echo "rules_perl already installed"
    return
  fi

  pushd ${HOME}/.local/build
  rm -rf rules_perl
  git clone https://github.com/bazelbuild/rules_perl.git
  pushd rules_perl
  git checkout tags/0.1.0 -b 0.1.0
  popd
  mv rules_perl ~/.local/lib
  popd
}

function setup_rules_python() {
  if [[ -d ${HOME}/.local/lib/rules_python ]]; then
    echo "rules_python already installed"
    return
  fi

  pushd ${HOME}/.local/build
  rm -rf rules_python
  git clone https://github.com/bazelbuild/rules_python.git
  pushd rules_python
  git checkout tags/0.25.0 -b 0.25.0
  popd
  mv rules_python ~/.local/lib
  popd
}

function setup_rules_apple() {
  if [[ -d ${HOME}/.local/lib/rules_apple ]]; then
    echo "rules_apple already installed"
    return
  fi

  pushd ${HOME}/.local/build
  rm -rf rules_apple
  git clone https://github.com/bazelbuild/rules_apple.git
  pushd rules_apple
  git checkout tags/0.32.0 -b 0.32.0
  popd
  mv rules_apple ~/.local/lib
  popd
}

function setup_rules_fuzzing() {
  if [[ -d ${HOME}/.local/lib/rules_fuzzing ]]; then
    echo "rules_fuzzing already installed"
    return
  fi

  pushd ${HOME}/.local/build
  rm -rf rules_fuzzing
  git clone https://github.com/bazelbuild/rules_fuzzing.git
  pushd rules_fuzzing
  git checkout tags/0.3.2 -b 0.3.2
  popd
  mv rules_fuzzing ~/.local/lib
  popd
}

function setup_gflags() {
  if [[ -d ${HOME}/.local/lib/gflags ]]; then
    echo "gflags already installed"
    return
  fi

  pushd ${HOME}/.local/build
  rm -rf gflags
  git clone https://github.com/gflags/gflags.git
  pushd gflags
  git checkout tags/v2.2.2 -b v2.2.2
  mkdir build
  cmake -DCMAKE_INSTALL_PREFIX=~/.local/lib/gflags -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-fPIC" -S . -B build
  cmake --build build -j10
  if [[ $? -ne 0 ]]; then
    echo "build gflags failed"
    exit 1
  fi
  cmake --build build --target install
  popd
}

function setup_glog() {
  if [[ -d ${HOME}/.local/lib/glog ]]; then
    echo "glog already installed"
    return
  fi

  pushd ${HOME}/.local/build
  rm -rf glog
  git clone https://github.com/google/glog.git
  pushd glog
  git checkout tags/v-1.6.0 -b v0.6.0
  mkdir build
  cmake -DCMAKE_INSTALL_PREFIX=~/.local/lib/glog -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -S . -B build
  cmake --build build -j10
  if [[ $? -ne 0 ]]; then
    echo "build glog failed"
    exit 1
  fi
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
  rm -rf googletest
  git clone https://github.com/google/googletest.git
  pushd googletest
  git checkout tags/v1.13.0 -b v1.13.0
  mkdir build
  cmake -DCMAKE_INSTALL_PREFIX=~/.local/lib/googletest -DCMAKE_BUILD_TYPE=Release -S . -B build
  cmake --build build -j10
  if [[ $? -ne 0 ]]; then
    echo "build googletest failed"
    exit 1
  fi
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
  rm -rf benchmark
  git clone https://github.com/google/benchmark.git
  pushd benchmark
  git checkout tags/v1.8.2 -b v1.8.2
  mkdir build
  cmake -DCMAKE_INSTALL_PREFIX=~/.local/lib/benchmark -DCMAKE_BUILD_TYPE=Release -DGOOGLETEST_PATH=../googletest -S . -B build
  cmake --build build -j10
  if [[ $? -ne 0 ]]; then
    echo "build google benchmark failed"
    exit 1
  fi
  cmake --build build --target install
  popd
  popd
}

function setup_tensorflow_cpu() {
  if [[ -d ${HOME}/.local/lib/libtensorflow ]]; then
    echo "tensorflow already installed"
    return
  fi

  pushd ${HOME}/.local/build
  rm -rf tensorflow
  git clone https://github.com/tensorflow/tensorflow.git
  pushd tensorflow
  git checkout tags/v2.13.0 -b v2.13.0
  yes '' | ./configure
  bazelisk clean --expunge
  uname=`uname`
  if [[ "${uname}" == "Darwin" ]]; then
    # bazelisk build --jobs=60 --compilation_mode=opt --spawn_strategy=sandboxed tensorflow/tools/lib_package:libtensorflow
    bazelisk build --jobs=60 --compilation_mode=opt -spawn_strategy=sandboxed --config=monolithic tensorflow/tools/lib_package:libtensorflow
  else
    # bazelisk build --jobs=60 --compilation_mode=opt --config=mkl --spawn_strategy=sandboxed tensorflow/tools/lib_package:libtensorflow
    bazelisk build --jobs=60 --compilation_mode=opt -spawn_strategy=sandboxed --config=monolithic tensorflow/tools/lib_package:libtensorflow
  fi
  if [[ $? -ne 0 ]]; then
    echo "build tensorflow failed"
    exit 1
  fi
  mkdir -p ~/.local/lib/libtensorflow
  tar zxvf bazel-bin/tensorflow/tools/lib_package/libtensorflow.tar.gz -C ~/.local/lib/libtensorflow
  cp -r bazel-bin/tensorflow/core/protobuf ~/.local/lib/libtensorflow/include/tensorflow/core
  cp -r bazel-bin/tensorflow/core/framework ~/.local/lib/libtensorflow/include/tensorflow/core
  cp -r bazel-bin/tensorflow/tsl ~/.local/lib/libtensorflow/include/tensorflow
  popd
  popd
}

function setup_tensorflow_gpu() {
  if [[ -d ${HOME}/.local/lib/libtensorflow_gpu ]]; then
    echo "tensorflow already installed"
    return
  fi

  export CUDA_TOOLKIT_PATH="/usr/local/cuda-11.8"
  export CUDNN_INSTALL_PATH="${HOME}/.local/lib/cudnn"
  pushd ${HOME}/.local/build
  rm -rf tensorflow
  git clone https://github.com/tensorflow/tensorflow.git
  pushd tensorflow
  git checkout tags/v2.13.0 -b v2.13.0
  ./configure
  bazelisk clean --expunge
  uname=`uname`
  bazelisk build --jobs=60 --config=monolithic --compilation_mode=opt --config=mkl tensorflow/tools/lib_package:libtensorflow
  if [[ $? -ne 0 ]]; then
    echo "build tensorflow failed"
    exit 1
  fi
  mkdir -p ~/.local/lib/libtensorflow_gpu
  tar zxvf bazel-bin/tensorflow/tools/lib_package/libtensorflow.tar.gz -C ~/.local/lib/libtensorflow_gpu
  cp -r bazel-bin/tensorflow/core/protobuf ~/.local/lib/libtensorflow_gpu/include/tensorflow/core
  cp -r bazel-bin/tensorflow/core/framework ~/.local/lib/libtensorflow_gpu/include/tensorflow/core
  cp -r bazel-bin/tensorflow/tsl ~/.local/lib/libtensorflow_gpu/include/tensorflow
  popd
  popd
}

function setup_tensorflow() {
  # Check if a GPU device is available
  if nvidia-smi &> /dev/null; then
    echo "GPU device found. Building TensorFlow with GPU support..."
    DEFAULT_SETUP_GPU=true
  else
    echo "No GPU device found. Building TensorFlow for CPU..."
    DEFAULT_SETUP_GPU=false
  fi

  setup_tensorflow_cpu
  if [[ ${SETUP_GPU} = true || ${DEFAULT_SETUP_GPU} = true ]]; then
    setup_tensorflow_gpu
  fi
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
  if [[ $? -ne 0 ]]; then
    echo "build zlib failed"
    exit 1
  fi
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
  if [[ $? -ne 0 ]]; then
    echo "build protobuf failed"
    exit 1
  fi
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
  if [[ $? -ne 0 ]]; then
    echo "build absl failed"
    exit 1
  fi
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
  rm -rf onnxruntime
  git clone https://github.com/microsoft/onnxruntime.git # or (https://github.com/intel/onnxruntime.git)
  pushd onnxruntime
  git checkout $(git describe --tags $(git rev-list --tags --max-count=1))

  uname=`uname`
  if [[ "${uname}" == "Darwin" ]]; then
    ./build.sh --config Release --build_shared_lib --parallel --compile_no_warning_as_error --skip_submodule_sync \
      --cmake_extra_defines CMAKE_INSTALL_PREFIX:PATH=~/.local/lib/onnxruntime                                    \
      --cmake_extra_defines CMAKE_OSX_ARCHITECTURES=arm64 &&                                                      \
    pushd build/MacOS/Release && make install && popd
  else
    ./build.sh --config Release --build_shared_lib --parallel --compile_no_warning_as_error \
      --cmake_extra_defines CMAKE_INSTALL_PREFIX:PATH=~/.local/lib/onnxruntime              \
      --cmake_extra_defines CMAKE_OSX_ARCHITECTURES=x86_64 &&                               \
    pushd build/Linux/Release && make install && popd
  fi
  if [[ $? -ne 0 ]]; then
    echo "build onnxruntime failed"
    exit 1
  fi
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
  rm -rf onnxruntime
  git clone https://github.com/microsoft/onnxruntime.git # or (https://github.com/intel/onnxruntime.git)
  pushd onnxruntime
  # git checkout $(git describe --tags $(git rev-list --tags --max-count=1))
  git checkout tags/v1.16.3 -b v1.16.3
  ./build.sh --config Release --build_shared_lib --parallel                                        \
    --cmake_extra_defines CMAKE_INSTALL_PREFIX:PATH=~/.local/lib/onnxruntime-mkl                   \
    --cmake_extra_defines CMAKE_CXX_FLAGS="-Wno-error=uninitialized -Wno-error=array-bounds -Wno-error=unused-variable -Wno-error=unknown-pragmas -Wno-error=unused-command-line-argument -DEIGEN_USE_MKL_ALL -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_sequential -lmkl_core -lstdc++ -lpthread -lm -lrt -ldl -lgomp" \
    --cmake_extra_defines CMAKE_C_FLAGS="-Wno-error=uninitialized -Wno-error=array-bounds -Wno-error=unused-variable -Wno-error=unknown-pragmas -Wno-error=unused-command-line-argument -DEIGEN_USE_MKL_ALL -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_sequential -lmkl_core -lstdc++ -lpthread -lm -lrt -ldl -lgomp"
  if [[ $? -ne 0 ]]; then
    echo "build onnxruntime-mkl failed"
    exit 1
  fi

  pushd build/Linux/Release && make install && popd
  popd
  popd
}

function setup_dnnl() {
  if ! [[ ${SETUP_ONNX_DNNL} = true || ${DEFAULT_SETUP_ONNX_DNNL} = true ]]; then
    echo "setup dnnl skipped, use SETUP_ONNX_DNNL=true to enable"
    return
  fi
  if [[ -d ${HOME}/.local/lib/dnnl ]]; then
    echo "dnnl already installed"
    return
  fi

  pushd ${HOME}/.local/build
  rm -rf dnnl
  git clone https://github.com/intel/mkl-dnn.git
  pushd dnnl
  # git checkout $(git describe --tags $(git rev-list --tags --max-count=1))
  git checkout tags/v3.3.3 -b v3.3.3
  cmake -DCMAKE_INSTALL_PREFIX=~/.local/lib/dnnl -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-fPIC" -S . -B build
  cmake --build build -j10
  if [[ $? -ne 0 ]]; then
    echo "build dnnl failed"
    exit 1
  fi
  make install
  pushd build/Linux/Release && make install && popd
  popd
  popd
}

function setup_onnx_dnnl() {
  if ! [[ ${SETUP_ONNX_DNNL} = true || ${DEFAULT_SETUP_ONNX_DNNL} = true ]]; then
    echo "setup onnxruntime_dnnl skipped, use SETUP_ONNX_DNNL=true to enable"
    return
  fi
  if [[ -d ${HOME}/.local/lib/onnxruntime_dnnl ]]; then
    echo "onnxruntime_dnnl already installed"
    return
  fi

  pushd ${HOME}/.local/build
  rm -rf onnxruntime
  git clone https://github.com/microsoft/onnxruntime.git # or (https://github.com/intel/onnxruntime.git)
  pushd onnxruntime
  # git checkout $(git describe --tags $(git rev-list --tags --max-count=1))
  git checkout tags/v1.16.3 -b v1.16.3
  ./build.sh --config Release --build_shared_lib --parallel --compile_no_warning_as_error --use_dnnl \
    --cmake_extra_defines CMAKE_INSTALL_PREFIX:PATH=~/.local/lib/onnxruntime_dnnl                    \
    --cmake_extra_defines CMAKE_OSX_ARCHITECTURES=x86_64
  if [[ $? -ne 0 ]]; then
    echo "build onnxruntime_dnnl failed"
    exit 1
  fi
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
  rm -rf onnxruntime
  git clone https://github.com/microsoft/onnxruntime.git # or (https://github.com/intel/onnxruntime.git)
  # git checkout $(git describe --tags $(git rev-list --tags --max-count=1))
  pushd onnxruntime
  git checkout tags/v1.16.3 -b v1.16.3
  ./build.sh --config Release --build_shared_lib --parallel --use_openvino CPU_FP32                \
    --cmake_extra_defines CMAKE_INSTALL_PREFIX:PATH=~/.local/lib/onnxruntime_openvino              \
    --cmake_extra_defines CMAKE_CXX_FLAGS="-Wno-error=maybe-uninitialized -Wno-error=array-bounds" \
    --cmake_extra_defines CMAKE_C_FLAGS="-Wno-error=maybe-uninitialized -Wno-error=array-bounds"
  if [[ $? -ne 0 ]]; then
    echo "build onnxruntime_openvino failed"
    exit 1
  fi
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
  rm -rf json
  git clone https://github.com/nlohmann/json.git
  pushd json
  git checkout tags/v3.11.2 -b v3.11.2
  cmake -DCMAKE_INSTALL_PREFIX=~/.local/lib/nlohmann_json -DCMAKE_BUILD_TYPE=Release -S . -B build
  cmake --build build -j10
  if [[ $? -ne 0 ]]; then
    echo "build nlohmann_json failed"
    exit 1
  fi
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
  rm -rf thread-pool
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
  rm -rf jemalloc
  git clone https://github.com/jemalloc/jemalloc.git
  pushd jemalloc
  git checkout tags/5.3.0 -b 5.3.0
  ./autogen.sh
  ./configure --prefix=${HOME}/.local/lib/jemalloc
  make -j10 && make install
  if [[ $? -ne 0 ]]; then
    echo "build jemalloc failed"
    exit 1
  fi
  popd
  popd
}

function setup_tcmalloc() {
  if [[ -d ${HOME}/.local/lib/tcmalloc ]]; then
    echo "tcmalloc already installed"
    return
  fi

  pushd ${HOME}/.local/build
  rm -rf tcmalloc
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
  setup_os
  setup_python
  setup_bazel
  setup_cmake
  setup_cpplint
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
  setup_dnnl
  setup_onnx
  setup_onnx_mkl
  setup_onnx_dnnl
  setup_onnx_openvino
  setup_skylib
  setup_rules_pkg
  setup_rules_foreign_cc
  setup_rules_perl
  setup_rules_python
  setup_rules_apple
  setup_rules_fuzzing
}
