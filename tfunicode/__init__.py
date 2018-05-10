from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import hashlib
import tensorflow as tf
import sysconfig
from os import path
from tensorflow.python.framework import ops

__VERSION__ = '1.4.3'


def __load_lib():
    uniq_flags = tf.sysconfig.get_compile_flags() + tf.sysconfig.get_link_flags() + [__VERSION__]
    uniq_flags = '/'.join(uniq_flags).encode('utf-8')
    flags_key = hashlib.md5(uniq_flags).hexdigest()

    ext_suffix = sysconfig.get_config_var('EXT_SUFFIX')
    if ext_suffix is None:
        ext_suffix = sysconfig.get_config_var('SO')

    lib_file = 'tfunicode_{}{}'.format(flags_key, ext_suffix)
    curr_dir = path.dirname(path.abspath(__file__))
    lib_path = path.join(curr_dir, '..', lib_file)

    if not path.exists(lib_path):
        raise Exception('OP library ({}) for your TF installation not found. '.format(lib_path) +
                        'Remove and install "tfunicode" package with --no-cache-dir option')

    return tf.load_op_library(lib_path)


_lib = __load_lib()


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
        result=tf.SparseTensor(
            indices=source.indices,
            values=_lib.transform_normalize_unicode(source.values, form),
            dense_shape=source.dense_shape
        )
    else:
        result = _lib.transform_normalize_unicode(source, form)

    return result


ops.NotDifferentiable("TransformNormalizeUnicode")


def transform_lower_case(source):
    """Lowercase strings tensor.

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to make lower.
    Returns:
        `Tensor` or `SparseTensor` of same shape and size as input.
    """

    source = tf.convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
    if isinstance(source, tf.SparseTensor):
        result=tf.SparseTensor(
            indices=source.indices,
            values=_lib.transform_lower_case(source.values),
            dense_shape=source.dense_shape
        )
    else:
        result = _lib.transform_lower_case(source)

    return result


ops.NotDifferentiable("TransformLowerCase")


def transform_upper_case(source):
    """Uppercase strings tensor.

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to make upper.
    Returns:
        `Tensor` or `SparseTensor` of same shape and size as input.
    """

    source = tf.convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
    if isinstance(source, tf.SparseTensor):
        result=tf.SparseTensor(
            indices=source.indices,
            values=_lib.transform_upper_case(source.values),
            dense_shape=source.dense_shape
        )
    else:
        result = _lib.transform_upper_case(source)

    return result


ops.NotDifferentiable("TransformUpperCase")


def transform_zero_digits(source):
    """Replace each digit with 0.

    Args:
        source: `Tensor` or `SparseTensor` of any shape, strings to replace digits.
    Returns:
        `Tensor` or `SparseTensor` of same shape and size as input.
    """

    source = tf.convert_to_tensor_or_sparse_tensor(source, dtype=tf.string)
    if isinstance(source, tf.SparseTensor):
        result=tf.SparseTensor(
            indices=source.indices,
            values=_lib.transform_zero_digits(source.values),
            dense_shape=source.dense_shape
        )
    else:
        result = _lib.transform_zero_digits(source)

    return result


ops.NotDifferentiable("TransformZeroDigits")


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
        result=tf.SparseTensor(
            indices=source.indices,
            values=_lib.transform_wrap_with(source.values, left, right),
            dense_shape=source.dense_shape
        )
    else:
        result = _lib.transform_wrap_with(source, left, right)

    return result


ops.NotDifferentiable("TransformWrapWith")


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
        child_indices, child_values, child_shape = _lib.expand_split_words(source.values)
        result = combine_sparse_successor(source.indices, source.dense_shape, child_indices, child_values, child_shape)
    else:
        indices, values, shape = _lib.expand_split_words(source)
        result = tf.SparseTensor(indices=indices, values=values, dense_shape=shape)

    return result


ops.NotDifferentiable("ExpandSplitWords")


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
        child_indices, child_values, child_shape = _lib.expand_split_chars(source.values)
        result = combine_sparse_successor(source.indices, source.dense_shape, child_indices, child_values, child_shape)
    else:
        indices, values, shape = _lib.expand_split_chars(source)
        result = tf.SparseTensor(indices=indices, values=values, dense_shape=shape)

    return result


ops.NotDifferentiable("ExpandSplitChars")


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
        child_indices, child_values, child_shape = _lib.expand_char_ngrams(source.values, minn, maxn, itself)
        result = combine_sparse_successor(source.indices, source.dense_shape, child_indices, child_values, child_shape)
    else:
        indices, values, shape = _lib.expand_char_ngrams(source, minn, maxn, itself)
        result = tf.SparseTensor(indices=indices, values=values, dense_shape=shape)

    return result


ops.NotDifferentiable("ExpandCharNgrams")


def combine_sparse_successor(parent_indices, parent_shape, child_indices, child_values, child_shape):
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

    indices, values, shape = _lib.cobine_sparse_successor(
        parent_indices,
        parent_shape,
        child_indices,
        child_values,
        child_shape
    )

    return tf.SparseTensor(indices=indices, values=values, dense_shape=shape)


ops.NotDifferentiable("CobineSparseSuccessor")
