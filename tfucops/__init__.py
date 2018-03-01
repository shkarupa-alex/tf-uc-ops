from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from os import path
from tensorflow.python.framework import ops
import tensorflow as tf


# Compiled kernels
_ops_lib = tf.load_op_library(path.join(path.dirname(path.abspath(__file__)), 'tfucops.so'))


def transform_normalize_unicode(source, form):
    """Normalize unicode strings tensor.

    Args:
        source: `Tensor` of any shape, strings to normalize.
        form: Scalar value, name of normalization algorithm.
            One of `"NFD"`, `"NFC"`, `"NFKD"`, `"NFKC"` (case-insensitive).
    Returns:
        `Tensor` of same shape and size as input.
    """

    source = tf.convert_to_tensor(source, dtype=tf.string)
    result = _ops_lib.transform_normalize_unicode(source, form)

    return result


ops.NotDifferentiable("TransformNormalizeUnicode")


def transform_lower_case(source):
    """Lowercase strings tensor.

    Args:
        source: `Tensor` of any shape, strings to make lower.
    Returns:
        `Tensor` of same shape and size as input.
    """

    source = tf.convert_to_tensor(source, dtype=tf.string)
    result = _ops_lib.transform_lower_case(source)

    return result


ops.NotDifferentiable("TransformLowerCase")


def transform_zero_digits(source):
    """Replace each digit with 0.

    Args:
        source: `Tensor` of any shape, strings to replace digits.
    Returns:
        `Tensor` of same shape and size as input.
    """

    source = tf.convert_to_tensor(source, dtype=tf.string)
    result = _ops_lib.transform_zero_digits(source)

    return result


ops.NotDifferentiable("TransformZeroDigits")


def expand_split_words(source, default_value=''):
    """Split unicode strings into words.
    Result tokens could be simply joined with empty separator to obtain original strings.

    Args:
        source: `Tensor` of any shape, strings to split
        default_value: Scalar value for padding.  Defaults to empty string.
    Returns:
        `Tensor` with an additional dimension of size 1 added.
    """

    source = tf.convert_to_tensor(source, dtype=tf.string)
    result = _ops_lib.expand_split_words(source, default_value)

    return result


ops.NotDifferentiable("ExpandSplitWords")


def expand_split_chars(source, default_value=''):
    """Split unicode strings into characters.
    Result tokens could be simply joined with empty separator to obtain original strings.

    Args:
        source: `Tensor` of any shape, strings to split
        default_value: Scalar value for padding.  Defaults to empty string.
    Returns:
        `Tensor` with an additional dimension of size 1 added.
    """

    source = tf.convert_to_tensor(source, dtype=tf.string)
    result = _ops_lib.expand_split_chars(source, default_value)

    return result


ops.NotDifferentiable("ExpandSplitChars")
