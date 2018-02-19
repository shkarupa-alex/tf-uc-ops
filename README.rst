Build Status .. image:: https://travis-ci.org/shkarupa-alex/tf-uc-ops.svg?branch=master

tf-uc-ops
=========
Package provide few custom OPs for TensorFlow to deal with unicode strings:
* transform_normalize_unicode
* transform_lower_case
* expand_split_words
* expand_split_chars

See test dir for usage examples.

Installation
------------
Package requires ICU c++ library to be installed.

For Linux distribution run
``sudo apt-get install -y g++ icu-devtools``

For MacOS X distribution run
``brew install icu4c``
and setup PKG_CONFIG_PATH as pointed in notice (e.g. export ``PKG_CONFIG_PATH="/usr/local/opt/icu4c/lib/pkgconfig:$PKG_CONFIG_PATH"``)
