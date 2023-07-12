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

# Install Tensorflow
git clone https://github.com/tensorflow/tensorflow.git
cd tensorflow
git checkout tags/v2.13.0 -b v2.13.0
./configure
bazelisk build --compilation_mode opt --config=mkl tensorflow/core:tensorflow
bazelisk build --compilation_mode opt --config=mkl tensorflow/tools/lib_package:libtensorflow
mkdir -p ~/.local/lib/libtensorflow
tar zxvf bazel-bin/tensorflow/tools/lib_package/libtensorflow.tar.gz -C ~/.local/lib/libtensorflow
