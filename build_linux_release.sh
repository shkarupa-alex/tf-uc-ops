#!/usr/bin/env bash

# Update system
apt-get update
apt-get install -y software-properties-common
add-apt-repository -y ppa:deadsnakes/ppa
apt-get update


# Python
apt-get install -y python-dev python-pip python3-dev python3-pip python3.6-dev
python3.6 -m pip install -U wheel==0.31.1 auditwheel==1.5.0

# Build tools
apt-get install -y unzip wget g++ gcc g++-4.8 gcc-4.8 patchelf rsync
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-5 60
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 40
update-alternatives --set g++ /usr/bin/g++-4.8
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 60
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 40
update-alternatives --set gcc /usr/bin/gcc-4.8


# Bazel
wget https://github.com/bazelbuild/bazel/releases/download/0.15.2/bazel-0.15.2-installer-linux-x86_64.sh
chmod u+x bazel-0.15.2-installer-linux-x86_64.sh
./bazel-0.15.2-installer-linux-x86_64.sh


# Build wheels
cp -R /tfunicode ~/tfunicode
cd ~/tfunicode

export PYTHON_BIN_PATH=`which python`
./configure.sh
bazel clean --expunge
bazel test --test_output=errors //tfunicode/...
bazel build build_pip_pkg
bazel-bin/build_pip_pkg /tfunicode/wheels

export PYTHON_BIN_PATH=`which python3`
./configure.sh
bazel clean --expunge
bazel test --test_output=errors //tfunicode/...
bazel build build_pip_pkg
bazel-bin/build_pip_pkg /tfunicode/wheels

export PYTHON_BIN_PATH=`which python3.6`
./configure.sh
bazel clean --expunge
bazel test --test_output=errors //tfunicode/...
bazel build build_pip_pkg
bazel-bin/build_pip_pkg /tfunicode/wheels
