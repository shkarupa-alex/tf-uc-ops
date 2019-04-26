# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from tfunicode.python.ops import transform_zero_digits
from tensorflow.python.framework import test_util
import tensorflow as tf


@test_util.run_all_in_graph_and_eager_modes
class TransformZeroDigitsTest(tf.test.TestCase):
    def testInferenceShape(self):
        source = [
            ['1', '2', '3'],
            ['4', '5', '6'],
        ]
        result = transform_zero_digits(source)

        self.assertEqual([2, 3], result.shape.as_list())

    def testActualShape(self):
        source = [
            ['1', '2', '3'],
            ['4', '5', '6'],
        ]
        result = transform_zero_digits(source)
        result = tf.shape(result)

        result = self.evaluate(result)
        self.assertEqual([2, 3], result.tolist())

    def testEmpty(self):
        result = transform_zero_digits('')

        result = self.evaluate(result)
        self.assertEqual(b'', result)

    def test0D(self):
        result = transform_zero_digits('7')

        result = self.evaluate(result)
        self.assertEqual(b'0', result)

    def test1D(self):
        result = transform_zero_digits(['7'])

        result = self.evaluate(result)
        self.assertEqual([b'0'], result)

    def test2D(self):
        result = transform_zero_digits([['7']])

        result = self.evaluate(result)
        self.assertEqual([[b'0']], result)

    def testSparse(self):
        expected = tf.SparseTensor(indices=[[0, 0]], values=[b'0'], dense_shape=[1, 1])
        source = tf.SparseTensor(indices=[[0, 0]], values=[b'7'], dense_shape=[1, 1])
        result = transform_zero_digits(source)

        expected, result = self.evaluate(expected), self.evaluate(result)
        self.assertAllEqual(expected.indices.tolist(), result.indices.tolist())
        self.assertAllEqual(expected.values.tolist(), result.values.tolist())
        self.assertAllEqual(expected.dense_shape.tolist(), result.dense_shape.tolist())

    def testMixedUnicode(self):
        result = transform_zero_digits(u'P.1, АБ1, ЯК12x')
        expected = tf.convert_to_tensor(u'P.0, АБ0, ЯК00x', dtype=tf.string)

        expected, result = self.evaluate(expected), self.evaluate(result)
        self.assertEqual(expected, result)


if __name__ == "__main__":
    tf.test.main()
