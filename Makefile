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


# TF binary flags
TF_CFLAGS := $(shell $(PYTHON) -c 'import tensorflow as tf; print(" ".join(tf.sysconfig.get_compile_flags()))')
TF_LFLAGS := $(shell $(PYTHON) -c 'import tensorflow as tf; print(" ".join(tf.sysconfig.get_link_flags()))')


# TF OS-specific flags
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	DARWIN_FLAGS := -undefined dynamic_lookup
endif


# ICU flags
ICU_FLAGS := $(shell pkg-config --libs --cflags icu-uc)


all:
  # cleanup
	$(PIP) uninstall -y tfucops || true
	rm -rf build dist tfucops.egg-info tfucops/tfucops.so

	# install dependencies
	$(PYTHON) setup.py egg_info
	$(PIP) install -U -r tfucops.egg-info/requires.txt

	# make extension
	g++ -std=c++11 -shared tfucops/src/tfucops.cc -o tfucops/tfucops.so -fPIC $(TF_CFLAGS) $(TF_LFLAGS) -O2 $(DARWIN_FLAGS) $(ABI_FLAGS) $(ICU_FLAGS)

	# install
	$(PYTHON) setup.py install --force

	# test
	$(PYTHON) tests/all_tests_run.py
