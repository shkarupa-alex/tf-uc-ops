#!/usr/bin/env bash

# Update system
apt-get update
apt-get install -y software-properties-common
add-apt-repository -y ppa:deadsnakes/ppa
apt-get update


# Python
apt-get install -y python-dev python-pip python-numpy python3-dev python3-pip python3.6-dev
python3.6 -m pip install -U wheel==0.31.1 auditwheel==1.5.0

#apt-get install -y python3-numpy
python3.5 -m pip install -U --force-reinstall numpy
python3.6 -m pip install -U --force-reinstall numpy

# Build tools
apt-get install -y unzip wget g++ gcc g++-4.8 gcc-4.8 patchelf
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
bazel build --cxxopt="-D_GLIBCXX_USE_CXX11_ABI=0" //pip_package
bazel-bin/pip_package/pip_package /tfunicode/wheels

export PYTHON_BIN_PATH=`which python3`
bazel build --cxxopt="-D_GLIBCXX_USE_CXX11_ABI=0" //pip_package
bazel-bin/pip_package/pip_package /tfunicode/wheels

# bazel clean  # if failed with numpy
export PYTHON_BIN_PATH=`which python3.6`
bazel build --cxxopt="-D_GLIBCXX_USE_CXX11_ABI=0" //pip_package
bazel-bin/pip_package/pip_package /tfunicode/wheels
