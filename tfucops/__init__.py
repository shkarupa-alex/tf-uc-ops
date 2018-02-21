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
    result = _ops_lib.normalize_unicode(source, form)

    return result


ops.NotDifferentiable("NormalizeUnicode")


def transform_lower_case(source):
    """Lowercase strings tensor.

    Args:
        source: `Tensor` of any shape, strings to make lower.
    Returns:
        `Tensor` of same shape and size as input.
    """

    source = tf.convert_to_tensor(source, dtype=tf.string)
    result = _ops_lib.lowercase_unicode(source)

    return result


ops.NotDifferentiable("LowercaseUnicode")


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
    result = _ops_lib.split_words(source, default_value)

    return result


ops.NotDifferentiable("SplitWords")
