#!/bin/bash

sudo apt-get update
sudo apt-get install -y            \
        apt-transport-https        \
        build-essential            \
        libcurl4-openssl-dev       \
        libcppunit-dev             \
        libunwind-dev              \
        libevent-dev               \
        libsasl2-dev               \
        libzstd-dev                \
        libssl-dev                 \
        libbz2-dev                 \
        liblz4-dev                 \
        zlib1g-dev                 \
        binutils-dev               \
        ant                        \
        g++                        \
        gcc                        \
        autoconf                   \
        libtool                    \
        automake                   \
        pkg-config                 \
        vim                        \
        wget                       \
        curl                       \
        gnupg                      \
        tree                       \
        psmisc                     \
        net-tools                  \
        nethogs                    \
        sysstat                    \
        iputils-ping               \
        htop                       \
        iotop                      \
        iftop                      \
        libgoogle-perftools-dev    \
        llvm                       \
        clang                      \
        git                        \
        libsqlite3-dev             \
        liblzma-dev                \
        libreadline-dev            \
        libbz2-dev

mkdir -p ${HOME}/.local/bin
curl https://pyenv.run | bash
echo '
export PATH="${HOME}/.local/bin:${HOME}/.pyenv/bin:${PATH}"
eval "$(pyenv init --path)"
eval "$(pyenv init -)"' >> ${HOME}/.bashrc
source ${HOME}/.bashrc
pyenv install 3.11.0
pyenv global 3.11.0

# Install by brew
# curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh | bash
# brew install gflags && cp -r /home/linuxbrew/.linuxbrew/Cellar/gflags ~/.local/lib
# brew install glog && cp -r /home/linuxbrew/.linuxbrew/Cellar/glog ~/.local/lib
# brew install libtensorflow && cp -r /home/linuxbrew/.linuxbrew/Cellar/libtensorflow/ ~/.local/lib

# Install cmake
wget https://github.com/Kitware/CMake/releases/download/v3.27.0-rc4/cmake-3.27.0-rc4.tar.gz
tar zxvf cmake-3.27.0-rc4.tar.gz
rm cmake-3.27.0-rc4.tar.gz
pushd cmake-3.27.0-rc4
./bootstrap --prefix=~/.local/cmake
make -j10 && make install
echo 'export PATH="${HOME}/.local/cmake/bin:${PATH}"' >> ~/.bashrc
source ~/.bashrc
popd

# Install gflags
git clone https://github.com/gflags/gflags.git
pushd gflags
git checkout tags/v2.2.2 -b v2.2.2
mkdir build
cmake -DCMAKE_INSTALL_PREFIX=~/.local/lib/gflags -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-fPIC" -S . -B build
cmake --build build -j10
cmake --build build --target install
popd

# Install glog
git clone https://github.com/google/glog.git
pushd glog
git checkout tags/v0.6.0 -b v0.6.0
mkdir build
cmake -DCMAKE_INSTALL_PREFIX=~/.local/lib/glog -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -S . -B build
cmake --build build -j10
cmake --build build --target install
popd

# Install googletest
git clone https://github.com/google/googletest.git
pushd googletest
git checkout tags/v1.13.0 -b v1.13.0

# Install google-benchmark
git clone https://github.com/google/benchmark.git
pushd benchmark
git checkout tags/v1.8.2 -b v1.8.2

# Install Tensorflow
git clone https://github.com/tensorflow/tensorflow.git
pushd tensorflow
git checkout tags/v2.13.0 -b v2.13.0
./configure
bazelisk build --jobs=10 --compilation_mode=opt --config=mkl tensorflow/tools/lib_package:libtensorflow
mkdir -p ~/.local/lib/libtensorflow
tar zxvf bazel-bin/tensorflow/tools/lib_package/libtensorflow.tar.gz -C ~/.local/lib/libtensorflow
cp -r bazel-bin/tensorflow/core/protobuf ~/.local/lib/libtensorflow/include/tensorflow/core
cp -r bazel-bin/tensorflow/core/framework ~/.local/lib/libtensorflow/include/tensorflow/core
cp -r bazel-bin/tensorflow/tsl ~/.local/lib/libtensorflow/include/tensorflow
popd

# Install zlib
cp -r tensorflow/bazel-tensorflow/external/zlib ./zlib
pushd zlib
./configure --prefix=~/.local/lib/zlib
make -j10 && make install
popd

# Install protobuf
cp -r tensorflow/bazel-tensorflow/external/com_google_protobuf ./protobuf
pushd protobuf
mkdir build
cmake -DCMAKE_INSTALL_PREFIX=~/.local/lib/protobuf -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release -S . -B build
cmake --build build -j10
cmake --build build --target install
popd

# Install abseil
cp -r tensorflow/bazel-tensorflow/external/com_google_absl ./protobuf
pushd com_google_absl
mkdir build
cmake -DCMAKE_INSTALL_PREFIX=~/.local/lib/absl -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=20 -S . -B build
cmake --build build -j10
cmake --build build --target install
popd

# Install onnxruntime
git clone https://github.com/microsoft/onnxruntime.git # or (https://github.com/intel/onnxruntime.git)
pushd onnxruntime
git checkout tags/v1.15.0 -b v1.15.0
./build.sh --config Release --build_shared_lib --parallel                                        \
  --cmake_extra_defines CMAKE_INSTALL_PREFIX:PATH=~/.local/lib/onnxruntime                       \
  --cmake_extra_defines CMAKE_CXX_FLAGS="-Wno-error=maybe-uninitialized -Wno-error=array-bounds" \
  --cmake_extra_defines CMAKE_C_FLAGS="-Wno-error=maybe-uninitialized -Wno-error=array-bounds"
pushd build/Linux/Release && make install && popd
# MacOS
# ./build.sh --config Release --build_shared_lib --parallel                                                       \
#   --cmake_extra_defines CMAKE_INSTALL_PREFIX:PATH=~/.local/lib/onnxruntime                                      \
#   --cmake_extra_defines CMAKE_CXX_FLAGS="-Wno-error=unused-but-set-variable -Wno-error=deprecated-declarations" \
#   --cmake_extra_defines CMAKE_C_FLAGS="-Wno-error=unused-but-set-variable -Wno-error=deprecated-declarations"
# pushd build/MacOS/Release && make install && popd
./build.sh --config Release --build_shared_lib --parallel --use_dnnl                             \
  --cmake_extra_defines CMAKE_INSTALL_PREFIX:PATH=~/.local/lib/onnxruntime_dnnl                  \
  --cmake_extra_defines CMAKE_CXX_FLAGS="-Wno-error=maybe-uninitialized -Wno-error=array-bounds" \
  --cmake_extra_defines CMAKE_C_FLAGS="-Wno-error=maybe-uninitialized -Wno-error=array-bounds"
pushd build/Linux/Release && make install && popd
./build.sh --config Release --build_shared_lib --parallel --use_openvino CPU_FP32                \
  --cmake_extra_defines CMAKE_INSTALL_PREFIX:PATH=~/.local/lib/onnxruntime_openvino              \
  --cmake_extra_defines CMAKE_CXX_FLAGS="-Wno-error=maybe-uninitialized -Wno-error=array-bounds" \
  --cmake_extra_defines CMAKE_C_FLAGS="-Wno-error=maybe-uninitialized -Wno-error=array-bounds"
pushd build/Linux/Release && make install && popd
popd

# Install nlohmann json
git clone https://github.com/nlohmann/json.git
pushd json
git checkout tags/v3.11.2 -b v3.11.2
cmake -DCMAKE_INSTALL_PREFIX=~/.local/lib/nlohmann_json -DCMAKE_BUILD_TYPE=Release -S . -B build
cmake --build build -j10
cmake --build build --target install
popd