#!/bin/bash

export PYTHON_BIN=python3
export PIP_BIN=pip3

apt-get -qq update
apt-get install -y pkg-config python3 python3-setuptools python3-pip g++ wget

wget -qO- http://download.icu-project.org/files/icu4c/60.2/icu4c-60_2-src.tgz | tar xvz
(cd icu/source && ./configure && make && make install)
export LD_LIBRARY_PATH="/usr/local/lib:$LD_LIBRARY_PATH"

cd /io/
sudo env LD_LIBRARY_PATH=$LD_LIBRARY_PATH PYTHON_BIN=$PYTHON_BIN PIP_BIN=$PIP_BIN make
