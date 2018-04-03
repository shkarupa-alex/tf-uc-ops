from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf


def dense_to_sparse(source, empty_value):
    if not isinstance(source, tf.Tensor):
        raise RuntimeError('source should be `Tensor`, got {}'.format(type(source)))

    indices = tf.where(tf.not_equal(source, empty_value))
    return indices
    # values = tf.gather_nd(source, indices)
    # shape = tf.shape(source, out_type=tf.int64)
    #
    # return tf.SparseTensor(indices=indices, values=values, dense_shape=shape)


def convert_to_sparse_tensor(source, empty_value):
    # if not isinstance(source, (tf.Tensor, tf.SparseTensor, tf.SparseTensorValue)):
    #     source = tf.convert_to_tensor(source, dtype=dtype)
    # empty = tf.constant(empty_value, dtype=dtype)

    result = tf.convert_to_tensor_or_sparse_tensor(source)

    if isinstance(result, tf.Tensor):
        # if not result.dtype.is_compatible_with(empty.dtype):
        #     raise RuntimeError("empty_value dtype: requested = %s, actual = %s" % (result.dtype.name, empty.dtype.name))
        #
        indices = tf.where(tf.not_equal(result, empty_value))
        values = tf.gather_nd(result, indices)
        shape = tf.shape(result, out_type=tf.int64)

        result = tf.SparseTensor(indices=indices, values=values, dense_shape=shape)

    return result
