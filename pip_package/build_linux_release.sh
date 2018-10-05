#!/usr/bin/env bash

apt-get update
add-apt-repository -y ppa:deadsnakes/ppa
apt-get update
apt-get install -y wget git mc pkg-config zip g++ gcc g++-4.8 gcc-4.8 zlib1g-dev unzip python patchelf
apt-get install -y python-numpy python-dev python-pip python-wheel
apt-get install -y python3-numpy python3-dev python3-pip python3-wheel
apt-get install -y python3.6 python3.6-dev
python3.6 -m pip install -U pip wheel==0.31.1 numpy auditwheel==1.5.0

update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-5 60
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 40
update-alternatives --set g++ /usr/bin/g++-4.8

update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 60
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 40
update-alternatives --set gcc /usr/bin/gcc-4.8

cp -R /tfunicode ~/tfunicode
cd ~/tfunicode

export PYTHON_BIN_PATH=python
bazel build --cxxopt="-D_GLIBCXX_USE_CXX11_ABI=0" //pip_package
bazel-bin/pip_package/pip_package /tfunicode/wheels

export PYTHON_BIN_PATH=python3
bazel build --cxxopt="-D_GLIBCXX_USE_CXX11_ABI=0" //pip_package
bazel-bin/pip_package/pip_package /tfunicode/wheels

export PYTHON_BIN_PATH=python3.6
bazel build --cxxopt="-D_GLIBCXX_USE_CXX11_ABI=0" //pip_package
bazel-bin/pip_package/pip_package /tfunicode/wheels
