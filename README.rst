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

RedHat-based linux
    ``sudo yum install gcc-c++ libicu-devel``

MacOS X
    ``brew install icu4c``
    
    setup PKG_CONFIG_PATH as pointed in notice (e.g. export ``PKG_CONFIG_PATH="/usr/local/opt/icu4c/lib/pkgconfig:$PKG_CONFIG_PATH"``)

2. Install package as ususal
    ``pip install -U tfucops``
