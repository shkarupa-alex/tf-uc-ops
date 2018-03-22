.. image:: https://travis-ci.org/shkarupa-alex/tf-uc-ops.svg?branch=master


tf-uc-ops
=========
Package provides few custom OPs for TensorFlow to deal with unicode strings:

* expand_split_words
* expand_split_chars
* expand_char_ngrams
* transform_normalize_unicode
* transform_lower_case
* transform_zero_digits
* transform_add_borders

See test dir for usage examples.

Requirements
------------
Package depends on ICU C++ library. Installation is OS-specific.

Debian-based linux
    ``sudo apt-get install -y pkg-config g++ wget``
    ``wget -qO- http://download.icu-project.org/files/icu4c/60.2/icu4c-60_2-src.tgz | tar xvz``
    ``(cd icu/source && ./configure && make && sudo make install)``

    Then setup LD_LIBRARY_PATH to point installed ICU shared library (e.g. ``export LD_LIBRARY_PATH="/usr/local/lib:$LD_LIBRARY_PATH"`` in ~/.profile)

MacOS X
    ``brew upgrade pkg-config icu4c``

    Then setup PKG_CONFIG_PATH as pointed in notice (e.g. ``PKG_CONFIG_PATH="/usr/local/opt/icu4c/lib/pkgconfig:$PKG_CONFIG_PATH"`` in ~/.profile)

Installation
------------
Install package as usual
    ``[sudo] pip install -U tfucops``

or clone this package & install manualy
    ``git clone https://github.com/shkarupa-alex/tfucops.git && cd tfucops``
    ``python setup.py install``
