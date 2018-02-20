# https://www.tensorflow.org/extend/adding_an_op#compile_the_op_using_your_system_compiler_tensorflow_binary_installation

# Env binaries
PYTHON = $(PYTHON_BIN)
ifeq ($(PYTHON),)
	PYTHON := python
endif

PIP = $(PIP_BIN)
ifeq ($(PIP),)
	PIP := pip
endif


# TF OS-specific flags
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	DARWIN_FLAGS := -undefined dynamic_lookup
endif


# ICU flags
ICU_FLAGS := $(shell pkg-config --libs --cflags icu-uc)


all: test

clean:
	$(PIP) uninstall -y tfucops || true
	$(PIP) uninstall -y tensorflow || true
	rm -rf build dist tfucops.egg-info tfucops/tfucops.so

deps: clean
	$(PYTHON) setup.py egg_info
	$(PIP) install -U -r tfucops.egg-info/requires.txt

build: deps
	$(eval TF_CFLAGS := $(shell $(PYTHON) -c 'import tensorflow as tf; print(" ".join(tf.sysconfig.get_compile_flags()))'))
	$(eval TF_LFLAGS := $(shell $(PYTHON) -c 'import tensorflow as tf; print(" ".join(tf.sysconfig.get_link_flags()))'))
	g++ -std=c++11 -shared tfucops/src/tfucops.cc -o tfucops/tfucops.so -fPIC $(TF_CFLAGS) $(TF_LFLAGS) -O2 $(DARWIN_FLAGS) $(ABI_FLAGS) $(ICU_FLAGS)

install: build
	$(PYTHON) setup.py install --force

test: install
	$(PYTHON) tests/all_tests_run.py
