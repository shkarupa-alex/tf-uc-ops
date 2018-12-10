package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "python/ops/_tfunicode_ops.so",
    srcs = [
        "cc/lib/expand_base.h",
        "cc/lib/expand_base.cc",
        "cc/lib/expand_shape.h",
        "cc/lib/expand_shape.cc",
        "cc/lib/transform_base.h",
        "cc/lib/transform_base.cc",
        "cc/lib/transform_shape.h",
        "cc/lib/transform_shape.cc",
        "cc/lib/word_break.h",
        "cc/lib/word_break.cc",
        "cc/kernels/combine_sparse_successor.cc",
        "cc/kernels/expand_char_ngrams.cc",
        "cc/kernels/expand_split_chars.cc",
        "cc/kernels/expand_split_words.cc",
        "cc/kernels/transform_lower_case.cc",
        "cc/kernels/transform_normalize_unicode.cc",
        "cc/kernels/transform_regex_replace.cc",
        "cc/kernels/transform_string_replace.cc",
        "cc/kernels/transform_title_case.cc",
        "cc/kernels/transform_upper_case.cc",
        "cc/kernels/transform_wrap_with.cc",
        "cc/kernels/transform_zero_digits.cc",
        "cc/ops/combine_sparse_successor.cc",
        "cc/ops/expand_char_ngrams.cc",
        "cc/ops/expand_split_chars.cc",
        "cc/ops/expand_split_words.cc",
        "cc/ops/transform_lower_case.cc",
        "cc/ops/transform_normalize_unicode.cc",
        "cc/ops/transform_regex_replace.cc",
        "cc/ops/transform_string_replace.cc",
        "cc/ops/transform_title_case.cc",
        "cc/ops/transform_upper_case.cc",
        "cc/ops/transform_wrap_with.cc",
        "cc/ops/transform_zero_digits.cc",
    ],
    linkshared = 1,
    deps = [
        "@local_config_tf//:libtensorflow_framework",
        "@local_config_tf//:tf_header_lib",
        "@re2//:re2",
        "@unilib//:unilib",
    ],
    copts = ["-pthread", "-std=c++11"]
)

py_library(
    name = "tfunicode_py",
    srcs = ([
        "__init__.py",
        "python/__init__.py",
        "python/ops/__init__.py",
    ]),
    data = [
        ":python/ops/_tfunicode_ops.so"
    ],
    srcs_version = "PY2AND3",
)


py_test(
    name = "combine_sparse_successor_test",
    srcs = ["python/kernel_tests/combine_sparse_successor_test.py"],
    srcs_version = "PY2AND3",
    deps = [":tfunicode_py"],
)

py_test(
    name = "expand_char_ngrams_test",
    srcs = ["python/kernel_tests/expand_char_ngrams_test.py"],
    srcs_version = "PY2AND3",
    deps = [":tfunicode_py"],
)

py_test(
    name = "expand_split_chars_test",
    srcs = ["python/kernel_tests/expand_split_chars_test.py"],
    srcs_version = "PY2AND3",
    deps = [":tfunicode_py"],
)

py_test(
    name = "expand_split_words_test",
    srcs = ["python/kernel_tests/expand_split_words_test.py"],
    srcs_version = "PY2AND3",
    deps = [":tfunicode_py"],
    data = ['python/kernel_tests/WordBreakTest.txt']
)

py_test(
    name = "transform_lower_case_test",
    srcs = ["python/kernel_tests/transform_lower_case_test.py"],
    srcs_version = "PY2AND3",
    deps = [":tfunicode_py"],
)

py_test(
    name = "transform_normalize_unicode_test",
    srcs = ["python/kernel_tests/transform_normalize_unicode_test.py"],
    srcs_version = "PY2AND3",
    deps = [":tfunicode_py"],
)

py_test(
    name = "transform_regex_replace_test",
    srcs = ["python/kernel_tests/transform_regex_replace_test.py"],
    srcs_version = "PY2AND3",
    deps = [":tfunicode_py"],
)

py_test(
    name = "transform_string_replace_test",
    srcs = ["python/kernel_tests/transform_string_replace_test.py"],
    srcs_version = "PY2AND3",
    deps = [":tfunicode_py"],
)

py_test(
    name = "transform_title_case_test",
    srcs = ["python/kernel_tests/transform_title_case_test.py"],
    srcs_version = "PY2AND3",
    deps = [":tfunicode_py"],
)

py_test(
    name = "transform_upper_case_test",
    srcs = ["python/kernel_tests/transform_upper_case_test.py"],
    srcs_version = "PY2AND3",
    deps = [":tfunicode_py"],
)

py_test(
    name = "transform_wrap_with_test",
    srcs = ["python/kernel_tests/transform_wrap_with_test.py"],
    srcs_version = "PY2AND3",
    deps = [":tfunicode_py"],
)

py_test(
    name = "transform_zero_digits_test",
    srcs = ["python/kernel_tests/transform_zero_digits_test.py"],
    srcs_version = "PY2AND3",
    deps = [":tfunicode_py"],
)
