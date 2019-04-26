# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from tfunicode.python.ops import transform_wrap_with
from tensorflow.python.framework import test_util
import tensorflow as tf


@test_util.run_all_in_graph_and_eager_modes
class TransformWrapWithTest(tf.test.TestCase):
    def testInferenceShape(self):
        source = [
            ['1', '2', '3'],
            ['4', '5', '6'],
        ]
        result = transform_wrap_with(source, '<', '>')

        self.assertEqual([2, 3], result.shape.as_list())

    def testActualShape(self):
        source = [
            ['1', '2', '3'],
            ['4', '5', '6'],
        ]
        result = transform_wrap_with(source, '<', '>')
        result = tf.shape(result)

        result = self.evaluate(result)
        self.assertEqual([2, 3], result.tolist())

    def testEmpty(self):
        result = transform_wrap_with('', '<', '>')

        result = self.evaluate(result)
        self.assertAllEqual(b'<>', result)

    def testEmptyBorders(self):
        result = transform_wrap_with('test', '', '')

        result = self.evaluate(result)
        self.assertAllEqual(b'test', result)

    def test0D(self):
        result = transform_wrap_with('X', '<', '>')

        result = self.evaluate(result)
        self.assertAllEqual(b'<X>', result)

    def test1D(self):
        result = transform_wrap_with(['X'], '<', '>')

        result = self.evaluate(result)
        self.assertAllEqual([b'<X>'], result)

    def test2D(self):
        result = transform_wrap_with([['X']], '<', '>')

        result = self.evaluate(result)
        self.assertAllEqual([[b'<X>']], result)

    def testSparse(self):
        expected = tf.SparseTensor(indices=[[0, 0]], values=[b'<X>'], dense_shape=[1, 1])
        source = tf.SparseTensor(indices=[[0, 0]], values=[b'X'], dense_shape=[1, 1])
        result = transform_wrap_with(source, '<', '>')

        expected, result = self.evaluate(expected), self.evaluate(result)
        self.assertAllEqual(expected.indices.tolist(), result.indices.tolist())
        self.assertAllEqual(expected.values.tolist(), result.values.tolist())
        self.assertAllEqual(expected.dense_shape.tolist(), result.dense_shape.tolist())

    def testUnicode(self):
        expected = u'надо'
        result = transform_wrap_with(u'ад', u'н', u'о')
        expected = tf.convert_to_tensor(expected, dtype=tf.string)

        expected, result = self.evaluate(expected), self.evaluate(result)
        self.assertAllEqual(expected, result)


if __name__ == "__main__":
    tf.test.main()
