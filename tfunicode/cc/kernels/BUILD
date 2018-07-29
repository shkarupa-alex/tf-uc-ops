package(default_visibility = ["//visibility:public"])

load("@org_tensorflow//tensorflow:tensorflow.bzl", "tf_kernel_library")


cc_library(
    name = "kernels",
    deps = [
        ":combine_sparse_successor",
        ":expand_char_ngrams",
        ":expand_split_chars",
        ":expand_split_words",
        ":transform_lower_case",
        ":transform_normalize_unicode",
        ":transform_upper_case",
        ":transform_wrap_with",
        ":transform_zero_digits",
    ],
)


tf_kernel_library(
    name = "combine_sparse_successor",
    srcs = ["combine_sparse_successor.cc"],
    deps = [
        "@org_tensorflow//tensorflow/core:framework_headers_lib",
        "@org_tensorflow//third_party/eigen3",
        "@protobuf_archive//:protobuf",
    ],
)


tf_kernel_library(
    name = "expand_char_ngrams",
    srcs = ["expand_char_ngrams.cc"],
    deps = [
        "@org_tensorflow//tensorflow/core:framework_headers_lib",
        "@org_tensorflow//third_party/eigen3",
        "@protobuf_archive//:protobuf",
        "//tfunicode/cc/lib:expand_base",
    ],
)


tf_kernel_library(
    name = "expand_split_chars",
    srcs = ["expand_split_chars.cc"],
    deps = [
        "@org_tensorflow//tensorflow/core:framework_headers_lib",
        "@org_tensorflow//third_party/eigen3",
        "@protobuf_archive//:protobuf",
        "//tfunicode/cc/lib:expand_base",
    ],
)


tf_kernel_library(
    name = "expand_split_words",
    srcs = ["expand_split_words.cc"],
    deps = [
        "@org_tensorflow//tensorflow/core:framework_headers_lib",
        "@org_tensorflow//third_party/eigen3",
        "@protobuf_archive//:protobuf",
        "//tfunicode/cc/lib:expand_base",
    ],
)


tf_kernel_library(
    name = "transform_lower_case",
    srcs = ["transform_lower_case.cc"],
    deps = [
        "@org_tensorflow//tensorflow/core:framework_headers_lib",
        "@org_tensorflow//third_party/eigen3",
        "@protobuf_archive//:protobuf",
        "@unilib//:unicode",
        "//tfunicode/cc/lib:transform_base",
   ],
)


tf_kernel_library(
    name = "transform_normalize_unicode",
    srcs = ["transform_normalize_unicode.cc"],
    deps = [
        "@org_tensorflow//tensorflow/core:framework_headers_lib",
        "@org_tensorflow//third_party/eigen3",
        "@protobuf_archive//:protobuf",
        "@unilib//:uninorms",
        "//tfunicode/cc/lib:transform_base",
    ],
)


tf_kernel_library(
    name = "transform_upper_case",
    srcs = ["transform_upper_case.cc"],
    deps = [
        "@org_tensorflow//tensorflow/core:framework_headers_lib",
        "@org_tensorflow//third_party/eigen3",
        "@protobuf_archive//:protobuf",
        "@unilib//:unicode",
        "//tfunicode/cc/lib:transform_base",
    ],
)


tf_kernel_library(
    name = "transform_wrap_with",
    srcs = ["transform_wrap_with.cc"],
    deps = [
        "@org_tensorflow//tensorflow/core:framework_headers_lib",
        "@org_tensorflow//third_party/eigen3",
        "@protobuf_archive//:protobuf",
         "//tfunicode/cc/lib:transform_base",
    ],
)


tf_kernel_library(
    name = "transform_zero_digits",
    srcs = ["transform_zero_digits.cc"],
    deps = [
        "@org_tensorflow//tensorflow/core:framework_headers_lib",
        "@org_tensorflow//third_party/eigen3",
        "@protobuf_archive//:protobuf",
        "@unilib//:unicode",
         "//tfunicode/cc/lib:transform_base",
    ],
)