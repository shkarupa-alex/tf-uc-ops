# https://www.tensorflow.org/extend/adding_an_op#compile_the_op_using_your_system_compiler_tensorflow_binary_installation

TF_CFLAGS := $(shell python -c 'import tensorflow as tf; print(" ".join(tf.sysconfig.get_compile_flags()))')
TF_LFLAGS := $(shell python -c 'import tensorflow as tf; print(" ".join(tf.sysconfig.get_link_flags()))')
ICU_FLAGS := $(shell pkg-config --libs --cflags icu-uc)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	OS_FLAGS := -undefined dynamic_lookup
endif

all:
	g++ -std=c++11 -shared tf-uc-ops/ops/ops.cc -o tf-uc-ops/ops/ops.so -fPIC $(TF_CFLAGS) $(TF_LFLAGS) -O2 $(OS_FLAGS) $(ICU_FLAGS)
	python -m tf-uc-ops.test
