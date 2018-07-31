package(default_visibility = ["//visibility:public"])

load("@org_tensorflow//tensorflow:tensorflow.bzl", "tf_gen_op_libs")
load("@org_tensorflow//tensorflow:tensorflow.bzl", "tf_custom_op_library")


cc_library(
    name = "cc",
    deps = [
        ":combine_sparse_successor_op_lib",
        "expand_char_ngrams_op_lib",
        "expand_split_chars_op_lib",
        "expand_split_words_op_lib",
        "transform_lower_case_op_lib",
        "transform_normalize_unicode_op_lib",
        "transform_upper_case_op_lib",
        "transform_wrap_with_op_lib",
        "transform_zero_digits_op_lib",
    ],
)

tf_gen_op_libs(
    op_lib_names = [
        "combine_sparse_successor",
        "expand_char_ngrams",
        "expand_split_chars",
        "expand_split_words",
        "transform_lower_case",
        "transform_normalize_unicode",
        "transform_upper_case",
        "transform_wrap_with",
        "transform_zero_digits",
    ],
    deps = [
        "//tfunicode/cc/lib:expand_shape",
        "//tfunicode/cc/lib:transform_shape",
        "//tfunicode/cc/kernels",
    ],
)

tf_custom_op_library(
    name = "ops/_tfunicode.so",
    srcs = [
        ":ops/combine_sparse_successor.cc",
        ":ops/expand_char_ngrams.cc",
        ":ops/expand_split_chars.cc",
        ":ops/expand_split_words.cc",
        ":ops/transform_lower_case.cc",
        ":ops/transform_normalize_unicode.cc",
        ":ops/transform_upper_case.cc",
        ":ops/transform_wrap_with.cc",
        ":ops/transform_zero_digits.cc",
    ],
    deps = [
        "//tfunicode/cc/lib:expand_shape",
        "//tfunicode/cc/lib:transform_shape",
        "//tfunicode/cc/kernels",
    ],
)
