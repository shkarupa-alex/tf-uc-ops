from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from tfunicode.python import ops_gen
from tensorflow.python.framework import load_library, ops
from tensorflow.python.platform import resource_loader
import os
import tensorflow as tf

_ops_lib = load_library.load_op_library(resource_loader.get_path_to_datafile(
    os.path.join('..', 'cc', 'ops', '_tfunicode.so')))

ops.NotDifferentiable("CobineSparseSuccessor")
ops.NotDifferentiable("ExpandCharNgrams")
ops.NotDifferentiable("ExpandSplitChars")
ops.NotDifferentiable("ExpandSplitWords")
ops.NotDifferentiable("TransformNormalizeUnicode")
ops.NotDifferentiable("TransformLowerCase")
ops.NotDifferentiable("TransformUpperCase")
ops.NotDifferentiable("TransformWrapWith")
ops.NotDifferentiable("TransformZeroDigits")


def transform_normalize_unicode(source, form):
    """Normalize unicode strings tensor.

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to normalize.
        form: Scalar value, name of normalization algorithm.
            One of `"NFD"`, `"NFC"`, `"NFKD"`, `"NFKC"`.
    Returns:
        `Tensor` or `SparseTensor` of same shape and size as input.
    """

    source = tf.convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
    if isinstance(source, tf.SparseTensor):
        result = tf.SparseTensor(
            indices=source.indices,
            values=ops_gen.transform_normalize_unicode(source.values, form),
            dense_shape=source.dense_shape
        )
    else:
        result = ops_gen.transform_normalize_unicode(source, form)

    return result


def transform_lower_case(source):
    """Lowercase strings tensor.

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to make lower.
    Returns:
        `Tensor` or `SparseTensor` of same shape and size as input.
    """

    source = tf.convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
    if isinstance(source, tf.SparseTensor):
        result = tf.SparseTensor(
            indices=source.indices,
            values=ops_gen.transform_lower_case(source.values),
            dense_shape=source.dense_shape
        )
    else:
        result = ops_gen.transform_lower_case(source)

    return result


def transform_upper_case(source):
    """Uppercase strings tensor.

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to make upper.
    Returns:
        `Tensor` or `SparseTensor` of same shape and size as input.
    """

    source = tf.convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
    if isinstance(source, tf.SparseTensor):
        result = tf.SparseTensor(
            indices=source.indices,
            values=ops_gen.transform_upper_case(source.values),
            dense_shape=source.dense_shape
        )
    else:
        result = ops_gen.transform_upper_case(source)

    return result


def transform_zero_digits(source):
    """Replace each digit with 0.

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to replace digits.
    Returns:
        `Tensor` or `SparseTensor` of same shape and size as input.
    """

    source = tf.convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
    if isinstance(source, tf.SparseTensor):
        result = tf.SparseTensor(
            indices=source.indices,
            values=ops_gen.transform_zero_digits(source.values),
            dense_shape=source.dense_shape
        )
    else:
        result = ops_gen.transform_zero_digits(source)

    return result


def transform_wrap_with(source, left, right):
    """Wrap source strings with "left" and "right" strings

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to replace digits.
        left: Scalar string to add in the beginning
        right: Scalar string to add in the ending
    Returns:
        `SparseTensor` of same shape and size as input.
    """

    source = tf.convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
    if isinstance(source, tf.SparseTensor):
        result = tf.SparseTensor(
            indices=source.indices,
            values=ops_gen.transform_wrap_with(source.values, left, right),
            dense_shape=source.dense_shape
        )
    else:
        result = ops_gen.transform_wrap_with(source, left, right)

    return result


def expand_split_words(source):
    """Split unicode strings into words.
    Result tokens could be simply joined with empty separator to obtain original strings.

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to split
    Returns:
        `SparseTensor` with an additional dimension of size 1 added.
    """

    source = tf.convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
    if isinstance(source, tf.SparseTensor):
        child_indices, child_values, child_shape = ops_gen.expand_split_words(source.values)
        result = _combine_sparse_successor(source.indices, source.dense_shape, child_indices, child_values, child_shape)
    else:
        indices, values, shape = ops_gen.expand_split_words(source)
        result = tf.SparseTensor(indices=indices, values=values, dense_shape=shape)

    return result


def expand_split_chars(source):
    """Split unicode strings into characters.
    Result tokens could be simply joined with empty separator to obtain original strings.

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to split
    Returns:
        `SparseTensor` with an additional dimension of size 1 added.
    """

    source = tf.convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
    if isinstance(source, tf.SparseTensor):
        child_indices, child_values, child_shape = ops_gen.expand_split_chars(source.values)
        result = _combine_sparse_successor(source.indices, source.dense_shape, child_indices, child_values, child_shape)
    else:
        indices, values, shape = ops_gen.expand_split_chars(source)
        result = tf.SparseTensor(indices=indices, values=values, dense_shape=shape)

    return result


def expand_char_ngrams(source, minn, maxn, itself='ASIS'):
    """Split unicode strings into char ngrams.
    Ngrams size configures with minn and max

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to split
        minn: Minimum length of char ngram
        minn: Maximum length of char ngram
        itself: Scalar value, strategy for source word preserving.
            One of `"ASIS"`, `"NEVER"`, `"ALWAYS"`.
    Returns:
        `SparseTensor` with an additional dimension of size 1 added.
    """

    source = tf.convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
    if isinstance(source, tf.SparseTensor):
        child_indices, child_values, child_shape = ops_gen.expand_char_ngrams(source.values, minn, maxn, itself)
        result = _combine_sparse_successor(source.indices, source.dense_shape, child_indices, child_values, child_shape)
    else:
        indices, values, shape = ops_gen.expand_char_ngrams(source, minn, maxn, itself)
        result = tf.SparseTensor(indices=indices, values=values, dense_shape=shape)

    return result


def _combine_sparse_successor(parent_indices, parent_shape, child_indices, child_values, child_shape):
    """Combines two string `SparseTensor`s, where second `SparseTensor` is the result of expanding
    first `SparseTensor`'s values.

    Args:
        parent_indices: 2D int64 `Tensor` with parent `SparseTensor` indices
        parent_shape: 1D int64 `Tensor` with parent `SparseTensor` dense_shape
        child_indices: 2D int64 `Tensor` with child `SparseTensor` indices
        child_values: 1D int64 `Tensor` with child `SparseTensor` values
        child_shape: 1D int64 `Tensor` with child `SparseTensor` dense_shape
    Returns:
        `SparseTensor` with an additional dimension of size 1 added.
    """

    indices, values, shape = ops_gen.cobine_sparse_successor(
        parent_indices,
        parent_shape,
        child_indices,
        child_values,
        child_shape
    )

    return tf.SparseTensor(indices=indices, values=values, dense_shape=shape)
