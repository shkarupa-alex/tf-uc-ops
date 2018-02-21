.. image:: https://travis-ci.org/shkarupa-alex/tf-uc-ops.svg?branch=master


tf-uc-ops
=========
Package provides few custom OPs for TensorFlow to deal with unicode strings:

* transform_normalize_unicode
* transform_lower_case
* expand_split_words
* expand_split_chars

See test dir for usage examples.

Installation
------------
1. Install GNU C++ Compiler and C++ ICU library. This step is OS-specific.

Debian-based linux
    ``sudo apt-get install -y g++ icu-devtools``
    ``sudo apt-get install -y pkg-config python3 python3-setuptools python3-pip g++ wget``
    ``wget -qO- http://download.icu-project.org/files/icu4c/60.2/icu4c-60_2-src.tgz | tar xvz``
    ``wget -qO- http://download.icu-project.org/files/icu4c/60.2/icu4c-60_2-src.tgz | tar xvz``
    ``cd icu/source && ./configure && make && sudo make install``

    setup LD_LIBRARY_PATH to point installed ICU shared library (e.g. ``export LD_LIBRARY_PATH="/usr/local/lib:$LD_LIBRARY_PATH"``)

MacOS X
    ``brew update``
    ``brew install pkg-config python gcc icu4c``
    ``brew upgrade pkg-config python gcc icu4c``

    setup PKG_CONFIG_PATH as pointed in notice (e.g. ``PKG_CONFIG_PATH="/usr/local/opt/icu4c/lib/pkgconfig:$PKG_CONFIG_PATH"``)

2. Setup python/pip, update pip/setuptools(any version supported by tensorflow)
    ``export PYTHON_BIN=python3``
    ``export PIP_BIN=pip3``

3. Clone this package, make & install
    ``git clone https://github.com/shkarupa-alex/tfucops.git && cd tfucops``
    [sudo] ``make``
