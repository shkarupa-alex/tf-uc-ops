#!/usr/bin/env bash

apt-get update
add-apt-repository -y ppa:deadsnakes/ppa
apt-get update
apt-get install wget git mc pkg-config zip g++ zlib1g-dev unzip python patchelf
apt-get install -y python-numpy python-dev python-pip python-wheel
apt-get install -y python3-numpy python3-dev python3-pip python3-wheel
apt-get install -y python3.6 python3.6-dev
python3.6 -m pip install -U pip wheel numpy auditwheel==1.5.0

git clone https://github.com/shkarupa-alex/tfunicode.git
cd tfunicode
git checkout develop

export PYTHON_BIN_PATH=python
bazel build --cxxopt="-D_GLIBCXX_USE_CXX11_ABI=0" //pip_package
bazel-bin/pip_package/pip_package /out

export PYTHON_BIN_PATH=python3
bazel build --cxxopt="-D_GLIBCXX_USE_CXX11_ABI=0" //pip_package
bazel-bin/pip_package/pip_package /out

export PYTHON_BIN_PATH=python3.6
bazel build --cxxopt="-D_GLIBCXX_USE_CXX11_ABI=0" //pip_package
bazel-bin/pip_package/pip_package /out
