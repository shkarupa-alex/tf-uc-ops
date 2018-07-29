package(default_visibility = ["//visibility:public"])

load("@org_tensorflow//tensorflow:tensorflow.bzl", "tf_custom_op_py_library", "tf_gen_op_wrapper_py")


py_library(
    name = "python",
    srcs = ["__init__.py"],
    srcs_version = "PY2AND3",
    deps = [
        ":ops_py_library",
    ],
)


tf_custom_op_py_library(
    name = "ops_py_library",
    srcs = ["ops.py"],
    dso = ["//tfunicode/cc:ops/_tfunicode.so"],
    kernels = ["//tfunicode/cc/kernels"],
    srcs_version = "PY2AND3",
    deps = [
        "@org_tensorflow//tensorflow:tensorflow_py",
        ":ops_py_wrapper",
    ],
)


tf_gen_op_wrapper_py(
    name = "ops_py_wrapper",
    out = "ops_gen.py",
    deps = ["//tfunicode/cc"],
)
