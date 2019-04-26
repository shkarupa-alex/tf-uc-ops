# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from tfunicode.python.ops import transform_regex_replace
from tensorflow.python.framework import test_util
import tensorflow as tf


@test_util.run_all_in_graph_and_eager_modes
class TransformWrapWithTest(tf.test.TestCase):
    def testInferenceShape(self):
        source = [
            ['1', '2', '3'],
            ['4', '5', '6'],
        ]
        result = transform_regex_replace(source, ['\\d'], ['0'])

        self.assertEqual([2, 3], result.shape.as_list())

    def testActualShape(self):
        source = [
            ['1', '2', '3'],
            ['4', '5', '6'],
        ]
        result = transform_regex_replace(source, ['\\d'], ['0'])
        result = tf.shape(result)

        result = self.evaluate(result)
        self.assertEqual([2, 3], result.tolist())

    def testEmpty(self):
        result = transform_regex_replace('', ['\\d'], ['0'])

        result = self.evaluate(result)
        self.assertAllEqual(b'', result)

    def testEmptyNeedle(self):
        with self.assertRaisesRegexp(tf.errors.InvalidArgumentError, 'Items of "pattern" could not be empty'):
            result = transform_regex_replace('<test>', [''], ['>'])
            result = self.evaluate(result)
            self.assertAllEqual(b'test', result)

    def testEmptyHaystack(self):
        result = transform_regex_replace('<test>', ['(<)'], [''])

        result = self.evaluate(result)
        self.assertAllEqual(b'test>', result)

    def test0D(self):
        result = transform_regex_replace('1test2', ['\\d'], ['0'])

        result = self.evaluate(result)
        self.assertAllEqual(b'0test0', result)

    def test1D(self):
        result = transform_regex_replace(['1test2'], ['\\d'], ['0'])

        result = self.evaluate(result)
        self.assertAllEqual([b'0test0'], result)

    def test2D(self):
        result = transform_regex_replace([['1test2']], ['\\d'], ['0'])

        result = self.evaluate(result)
        self.assertAllEqual([[b'0test0']], result)

    def testSparse(self):
        expected = tf.SparseTensor(indices=[[0, 0]], values=[b'0test0'], dense_shape=[1, 1])
        source = tf.SparseTensor(indices=[[0, 0]], values=[b'1test2'], dense_shape=[1, 1])
        result = transform_regex_replace(source, ['\\d'], ['0'])

        expected, result = self.evaluate(expected), self.evaluate(result)
        self.assertAllEqual(expected.indices.tolist(), result.indices.tolist())
        self.assertAllEqual(expected.values.tolist(), result.values.tolist())
        self.assertAllEqual(expected.dense_shape.tolist(), result.dense_shape.tolist())

    def testUnicode(self):
        expected = u'_ на _ он же _-0 _ _̈_ _'
        result = transform_regex_replace(
            u'тест на юникод он же utf-8 плюс совмещённый символ',
            [u'\\pL{3,}', u'\\d'],
            ['_', u'0']
        )
        expected = tf.convert_to_tensor(expected, dtype=tf.string)

        expected, result = self.evaluate(expected), self.evaluate(result)
        self.assertAllEqual(expected, result)


if __name__ == "__main__":
    tf.test.main()
