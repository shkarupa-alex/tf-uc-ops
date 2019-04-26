from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
from tensorflow.python.framework import load_library, ops
from tensorflow.python.platform import resource_loader
from tensorflow.python.framework.sparse_tensor import convert_to_tensor_or_sparse_tensor

ops_module = load_library.load_op_library(resource_loader.get_path_to_datafile('_tfunicode_ops.so'))

ops.NotDifferentiable('CombineSparseSuccessor')
ops.NotDifferentiable('ExpandCharNgrams')
ops.NotDifferentiable('ExpandSplitChars')
ops.NotDifferentiable('ExpandSplitWords')
ops.NotDifferentiable('TransformLowerCase')
ops.NotDifferentiable('TransformNormalizeUnicode')
ops.NotDifferentiable('TransformRegexReplace')
ops.NotDifferentiable('TransformStringReplace')
ops.NotDifferentiable('TransformTitleCase')
ops.NotDifferentiable('TransformUpperCase')
ops.NotDifferentiable('TransformWrapWith')
ops.NotDifferentiable('TransformZeroDigits')


def _combine_sparse_successor(parent_indices, parent_shape, child_indices, child_values, child_shape, name=None):
    """Combines two string `SparseTensor`s, where second `SparseTensor` is the result of expanding
    first `SparseTensor`'s values.

    Args:
        parent_indices: 2D int64 `Tensor` with parent `SparseTensor` indices
        parent_shape: 1D int64 `Tensor` with parent `SparseTensor` dense_shape
        child_indices: 2D int64 `Tensor` with child `SparseTensor` indices
        child_values: 1D int64 `Tensor` with child `SparseTensor` values
        child_shape: 1D int64 `Tensor` with child `SparseTensor` dense_shape
        name: A name for the operation (optional).
    Returns:
        `SparseTensor` with an additional dimension of size 1 added.
    """

    with ops.name_scope(name, "CombineSparseSuccessor",
                        [parent_indices, parent_shape, child_indices, child_values, child_shape]):
        indices, values, shape = ops_module.combine_sparse_successor(
            parent_indices,
            parent_shape,
            child_indices,
            child_values,
            child_shape
        )

        return tf.SparseTensor(indices=indices, values=values, dense_shape=shape)


def expand_char_ngrams(source, minn, maxn, itself='ASIS', name=None):
    """Split unicode strings into char ngrams.
    Ngrams size configures with minn and max

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to split
        minn: Minimum length of char ngram
        minn: Maximum length of char ngram
        itself: Scalar value, strategy for source word preserving.
            One of `"ASIS"`, `"NEVER"`, `"ALWAYS"`, `"ALONE"`.
        name: A name for the operation (optional).
    Returns:
        `SparseTensor` with an additional dimension of size 1 added.
    """

    with ops.name_scope(name, "ExpandCharNgrams", [source]):
        source = convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
        if isinstance(source, tf.SparseTensor):
            child_indices, child_values, child_shape = ops_module.expand_char_ngrams(source.values, minn, maxn, itself)
            result = _combine_sparse_successor(source.indices, source.dense_shape, child_indices, child_values,
                                               child_shape)
        else:
            indices, values, shape = ops_module.expand_char_ngrams(source, minn, maxn, itself)
            result = tf.SparseTensor(indices=indices, values=values, dense_shape=shape)

        return result


def expand_split_chars(source, name=None):
    """Split unicode strings into characters.
    Result tokens could be simply joined with empty separator to obtain original strings.

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to split
        name: A name for the operation (optional).
    Returns:
        `SparseTensor` with an additional dimension of size 1 added.
    """

    with ops.name_scope(name, "ExpandSplitChars", [source]):
        source = convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
        if isinstance(source, tf.SparseTensor):
            child_indices, child_values, child_shape = ops_module.expand_split_chars(source.values)
            result = _combine_sparse_successor(source.indices, source.dense_shape, child_indices, child_values,
                                               child_shape)
        else:
            indices, values, shape = ops_module.expand_split_chars(source)
            result = tf.SparseTensor(indices=indices, values=values, dense_shape=shape)

        return result


def expand_split_words(source, extended=False, name=None):
    """Split unicode strings into words.
    Result tokens could be simply joined with empty separator to obtain original strings.

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to split
        extended: Boolean flag, should rules WB6 and WB7 break on "stop" marks
        name: A name for the operation (optional).
    Returns:
        `SparseTensor` with an additional dimension of size 1 added.
    """

    with ops.name_scope(name, "ExpandSplitWords", [source]):
        source = convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
        if isinstance(source, tf.SparseTensor):
            child_indices, child_values, child_shape = ops_module.expand_split_words(source.values, extended)
            result = _combine_sparse_successor(source.indices, source.dense_shape, child_indices, child_values,
                                               child_shape)
        else:
            indices, values, shape = ops_module.expand_split_words(source, extended)
            result = tf.SparseTensor(indices=indices, values=values, dense_shape=shape)

        return result


def transform_normalize_unicode(source, form, name=None):
    """Normalize unicode strings tensor.

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to normalize.
        form: Scalar value, name of normalization algorithm.
            One of `"NFD"`, `"NFC"`, `"NFKD"`, `"NFKC"`.
        name: A name for the operation (optional).
    Returns:
        `Tensor` or `SparseTensor` of same shape and size as input.
    """

    with ops.name_scope(name, "TransformNormalizeUnicode", [source]):
        source = convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
        if isinstance(source, tf.SparseTensor):
            result = tf.SparseTensor(
                indices=source.indices,
                values=ops_module.transform_normalize_unicode(source.values, form),
                dense_shape=source.dense_shape
            )
        else:
            result = ops_module.transform_normalize_unicode(source, form)

        return result


def transform_lower_case(source, name=None):
    """Lowercase strings tensor.

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to make lower.
        name: A name for the operation (optional).
    Returns:
        `Tensor` or `SparseTensor` of same shape and size as input.
    """

    with ops.name_scope(name, "TransformLowerCase", [source]):
        source = convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
        if isinstance(source, tf.SparseTensor):
            result = tf.SparseTensor(
                indices=source.indices,
                values=ops_module.transform_lower_case(source.values),
                dense_shape=source.dense_shape
            )
        else:
            result = ops_module.transform_lower_case(source)

        return result


def transform_regex_replace(source, pattern, rewrite, name=None):
    """Replace all substrings from `needle` to corresponding strings in `haystack` with source.

    Args:
        source: `Tensor` or `SparseTensor` of any shape, source strings for replacing.
        pattern: List of RE2 patterns to search in source
        rewrite: List of strings to replace with. Should have same length as `needle`.
        name: A name for the operation (optional).
    Returns:
        `Tensor` or `SparseTensor` of same shape and size as input.
    """

    with ops.name_scope(name, "TransformRegexReplace", [source]):
        source = convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
        if isinstance(source, tf.SparseTensor):
            result = tf.SparseTensor(
                indices=source.indices,
                values=ops_module.transform_regex_replace(source.values, pattern, rewrite),
                dense_shape=source.dense_shape
            )
        else:
            result = ops_module.transform_regex_replace(source, pattern, rewrite)

        return result


def transform_string_replace(source, needle, haystack, name=None):
    """Replace all substrings from `needle` to corresponding strings in `haystack` with source.

    Args:
        source: `Tensor` or `SparseTensor` of any shape, source strings for replacing.
        needle: List of strings to search in source
        haystack: List of strings to replace with. Should have same length as `needle`.
        name: A name for the operation (optional).
    Returns:
        `Tensor` or `SparseTensor` of same shape and size as input.
    """

    with ops.name_scope(name, "TransformStringReplace", [source]):
        source = convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
        if isinstance(source, tf.SparseTensor):
            result = tf.SparseTensor(
                indices=source.indices,
                values=ops_module.transform_string_replace(source.values, needle, haystack),
                dense_shape=source.dense_shape
            )
        else:
            result = ops_module.transform_string_replace(source, needle, haystack)

    return result


def transform_title_case(source, name=None):
    """Titlecase strings tensor.

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to make title.
        name: A name for the operation (optional).
    Returns:
        `Tensor` or `SparseTensor` of same shape and size as input.
    """

    with ops.name_scope(name, "TransformTitleCase", [source]):
        source = convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
        if isinstance(source, tf.SparseTensor):
            result = tf.SparseTensor(
                indices=source.indices,
                values=ops_module.transform_title_case(source.values),
                dense_shape=source.dense_shape
            )
        else:
            result = ops_module.transform_title_case(source)

        return result


def transform_upper_case(source, name=None):
    """Uppercase strings tensor.

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to make upper.
        name: A name for the operation (optional).
    Returns:
        `Tensor` or `SparseTensor` of same shape and size as input.
    """

    with ops.name_scope(name, "TransformUpperCase", [source]):
        source = convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
        if isinstance(source, tf.SparseTensor):
            result = tf.SparseTensor(
                indices=source.indices,
                values=ops_module.transform_upper_case(source.values),
                dense_shape=source.dense_shape
            )
        else:
            result = ops_module.transform_upper_case(source)

        return result


def transform_zero_digits(source, name=None):
    """Replace each digit with 0.

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to replace digits.
        name: A name for the operation (optional).
    Returns:
        `Tensor` or `SparseTensor` of same shape and size as input.
    """

    with ops.name_scope(name, "TransformZeroDigits", [source]):
        source = convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
        if isinstance(source, tf.SparseTensor):
            result = tf.SparseTensor(
                indices=source.indices,
                values=ops_module.transform_zero_digits(source.values),
                dense_shape=source.dense_shape
            )
        else:
            result = ops_module.transform_zero_digits(source)

        return result


def transform_wrap_with(source, left, right, name=None):
    """Wrap source strings with "left" and "right" strings

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to replace digits.
        left: Scalar string to add in the beginning
        right: Scalar string to add in the ending
        name: A name for the operation (optional).
    Returns:
        `SparseTensor` of same shape and size as input.
    """

    with ops.name_scope(name, "TransformWrapWith", [source]):
        source = convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
        if isinstance(source, tf.SparseTensor):
            result = tf.SparseTensor(
                indices=source.indices,
                values=ops_module.transform_wrap_with(source.values, left, right),
                dense_shape=source.dense_shape
            )
        else:
            result = ops_module.transform_wrap_with(source, left, right)

        return result
