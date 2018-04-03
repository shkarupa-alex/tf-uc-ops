from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import hashlib
import tensorflow as tf
import sysconfig
from os import path
from tensorflow.python.framework import ops

__VERSION__ = '1.4.0'


def __load_lib():
    uniq_flags = tf.sysconfig.get_compile_flags() + tf.sysconfig.get_link_flags() + [__VERSION__]
    uniq_flags = '/'.join(uniq_flags).encode('utf-8')
    flags_key = hashlib.md5(uniq_flags).hexdigest()

    ext_suffix = sysconfig.get_config_var('EXT_SUFFIX')
    if ext_suffix is None:
        ext_suffix = sysconfig.get_config_var('SO')

    lib_file = 'tfucops_{}{}'.format(flags_key, ext_suffix)
    curr_dir = path.dirname(path.abspath(__file__))
    lib_path = path.join(curr_dir, '..', lib_file)

    if not path.exists(lib_path):
        raise Exception('OP library ({}) for your TF installation not found. '.format(lib_path) +
                        'Remove and install with "tfucops" package with --no-cache-dir option')

    return tf.load_op_library(lib_path)


_lib = __load_lib()


def transform_normalize_unicode(source, form):
    """Normalize unicode strings tensor.

    Args:
        source: `Tensor` of any shape, strings to normalize.
        form: Scalar value, name of normalization algorithm.
            One of `"NFD"`, `"NFC"`, `"NFKD"`, `"NFKC"`.
    Returns:
        `Tensor` of same shape and size as input.
    """

    source = tf.convert_to_tensor(source, dtype=tf.string)
    result = _lib.transform_normalize_unicode(source, form)

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
    result = _lib.transform_lower_case(source)

    return result


ops.NotDifferentiable("TransformLowerCase")


def transform_upper_case(source):
    """Uppercase strings tensor.

    Args:
        source: `Tensor` of any shape, strings to make upper.
    Returns:
        `Tensor` of same shape and size as input.
    """

    source = tf.convert_to_tensor(source, dtype=tf.string)
    result = _lib.transform_upper_case(source)

    return result


ops.NotDifferentiable("TransformUpperCase")


def transform_zero_digits(source):
    """Replace each digit with 0.

    Args:
        source: `Tensor` of any shape, strings to replace digits.
    Returns:
        `Tensor` of same shape and size as input.
    """

    source = tf.convert_to_tensor(source, dtype=tf.string)
    result = _lib.transform_zero_digits(source)

    return result


ops.NotDifferentiable("TransformZeroDigits")


def transform_wrap_with(source, left, right):
    """Wrap source strings with "left" and "right" strings

    Args:
        source: `Tensor` of any shape, strings to replace digits.
        left: Scalar string to add in the beginning
        right: Scalar string to add in the ending
    Returns:
        `Tensor` of same shape and size as input.
    """

    source = tf.convert_to_tensor(source, dtype=tf.string)
    result = _lib.transform_wrap_with(source, left, right)

    return result


ops.NotDifferentiable("TransformWrapWith")


def expand_split_words(source):
    """Split unicode strings into words.
    Result tokens could be simply joined with empty separator to obtain original strings.

    Args:
        source: `Tensor` of any shape, strings to split
    Returns:
        `SparseTensor` with an additional dimension of size 1 added.
    """

    source = tf.convert_to_tensor(source, dtype=tf.string)
    indices, values, shape = _lib.expand_split_words(source)

    return tf.SparseTensor(indices=indices, values=values, dense_shape=shape)


ops.NotDifferentiable("ExpandSplitWords")


def expand_split_chars(source):
    """Split unicode strings into characters.
    Result tokens could be simply joined with empty separator to obtain original strings.

    Args:
        source: `Tensor` of any shape, strings to split
    Returns:
        `SparseTensor` with an additional dimension of size 1 added.
    """

    source = tf.convert_to_tensor(source, dtype=tf.string)
    indices, values, shape = _lib.expand_split_chars(source)

    return tf.SparseTensor(indices=indices, values=values, dense_shape=shape)


ops.NotDifferentiable("ExpandSplitChars")


def expand_char_ngrams(source, minn, maxn, itself='ASIS'):
    """Split unicode strings into char ngrams.
    Ngrams size configures with minn and max

    Args:
        source: `Tensor` of any shape, strings to split
        minn: Minimum length of char ngram
        minn: Maximum length of char ngram
        itself: Scalar value, strategy for source word preserving.
            One of `"ASIS"`, `"NEVER"`, `"ALWAYS"`.
    Returns:
        `SparseTensor` with an additional dimension of size 1 added.
    """

    source = tf.convert_to_tensor(source, dtype=tf.string)
    indices, values, shape = _lib.expand_char_ngrams(source, minn, maxn, itself)

    return tf.SparseTensor(indices=indices, values=values, dense_shape=shape)


ops.NotDifferentiable("ExpandCharNgrams")
