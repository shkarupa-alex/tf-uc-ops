# https://www.tensorflow.org/extend/adding_an_op#compile_the_op_using_your_system_compiler_tensorflow_binary_installation

TF_CFLAGS := $(shell env python -c 'import tensorflow as tf; print(" ".join(tf.sysconfig.get_compile_flags()))')
TF_LFLAGS := $(shell env python -c 'import tensorflow as tf; print(" ".join(tf.sysconfig.get_link_flags()))')
ICU_FLAGS := $(shell pkg-config --libs --cflags icu-uc)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	OS_FLAGS := -undefined dynamic_lookup
endif

test:
	env python -m tests

build: clean
	g++ -std=c++11 -shared tfucops/src/tf_uc_ops.cc -o tfucops/tf_uc_ops.so -fPIC $(TF_CFLAGS) $(TF_LFLAGS) -O2 $(OS_FLAGS) $(ICU_FLAGS)

clean:
	rm -f tf-uc-ops/ops.so
