#!/bin/bash
set -e -x


# Install a system package required by ours
echo "$TRAVIS_OS_NAME"
if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
    sudo apt-get -qq update
    sudo apt-get install -y g++ icu-devtools
fi
if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
    brew update
    brew install icu4c
    export PKG_CONFIG_PATH="/usr/local/opt/icu4c/lib/pkgconfig:$PKG_CONFIG_PATH"
fi


# Compile wheels
for PYBIN in /opt/python/*/bin; do
    "${PYBIN}/python" /io/setup.py install_egg_info
    "${PYBIN}/pip" install -r /io/tfucops.egg-info/requires.txt
    make -f/io/Makefile build
    "${PYBIN}/pip" wheel /io/ -w wheelhouse/
done

# Bundle external shared libraries into the wheels
for whl in wheelhouse/*.whl; do
    auditwheel repair "$whl" -w /io/dist/
done

# Install packages and test
for PYBIN in /opt/python/*/bin/; do
    "${PYBIN}/pip" install tfucops --no-index -f /io/dist
    (cd /io/; ${PYBIN} -m tests)
done
