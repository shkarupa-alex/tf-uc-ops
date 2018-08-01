package(default_visibility = ["//visibility:public"])

load("@org_tensorflow//tensorflow:tensorflow.bzl", "tf_custom_op_library_additional_deps")


cc_library(
    name = "expand_base",
    srcs = ["expand_base.cc"],
    hdrs = ["expand_base.h"],
    deps = ["@unilib//:utf8"] + tf_custom_op_library_additional_deps(),
)


cc_library(
    name = "expand_shape",
    srcs = ["expand_shape.cc"],
    hdrs = ["expand_shape.h"],
    deps = tf_custom_op_library_additional_deps(),
)


cc_library(
    name = "transform_base",
    srcs = ["transform_base.cc"],
    hdrs = ["transform_base.h"],
    deps = ["@unilib//:utf8"] + tf_custom_op_library_additional_deps(),
)


cc_library(
    name = "transform_shape",
    srcs = ["transform_shape.cc"],
    hdrs = ["transform_shape.h"],
    deps = tf_custom_op_library_additional_deps(),
)

cc_library(
    name = "word_break",
    srcs = ["word_break.cc"],
    hdrs = ["word_break.h"],
    deps = ["@unilib//:unicode"] + tf_custom_op_library_additional_deps(),
)
