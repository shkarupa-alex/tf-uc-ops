# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from tfunicode.python.ops import expand_char_ngrams
from tensorflow.python.framework import test_util
import tensorflow as tf


@test_util.run_all_in_graph_and_eager_modes
class ExpandCharNgramsTest(tf.test.TestCase):
    def testInferenceShape(self):
        source = [
            ['1', '2', '3'],
            ['4', '5', '6'],
        ]
        result = expand_char_ngrams(source, 1, 1, itself='ALWAYS')

        if tf.executing_eagerly():
            self.assertEqual([6, 3], result.indices.shape.as_list())
            self.assertEqual([6], result.values.shape.as_list())
            self.assertEqual([3], result.dense_shape.shape.as_list())
        else:
            self.assertEqual([None, 3], result.indices.shape.as_list())
            self.assertEqual([None], result.values.shape.as_list())
            self.assertEqual([3], result.dense_shape.shape.as_list())

    def testActualShape(self):
        source = [
            ['1', '2', '3'],
            ['4', '5', '6'],
        ]
        result = expand_char_ngrams(source, 1, 1)

        result = self.evaluate(result)
        self.assertAllEqual([2, 3, 1], result.dense_shape)

    def testEmpty(self):
        expected = tf.convert_to_tensor([], dtype=tf.string)
        result = expand_char_ngrams('', 1, 1)
        result = tf.sparse.to_dense(result, default_value='')

        expected, result = self.evaluate(expected), self.evaluate(result)
        self.assertAllEqual(expected, result)

    def test0D(self):
        expected = tf.convert_to_tensor(['x', 'y'], dtype=tf.string)
        result = expand_char_ngrams('xy', 1, 1)
        result = tf.sparse.to_dense(result, default_value='')

        expected, result = self.evaluate(expected), self.evaluate(result)
        self.assertAllEqual(expected, result)

    def test1D(self):
        expected = tf.convert_to_tensor([['x', 'y']], dtype=tf.string)
        result = expand_char_ngrams(['xy'], 1, 1)
        result = tf.sparse.to_dense(result, default_value='')

        expected, result = self.evaluate(expected), self.evaluate(result)
        self.assertAllEqual(expected, result)

    def testDefault2D(self):
        expected = tf.convert_to_tensor([[
            ['x', 'y'],
            ['x', '']
        ]], dtype=tf.string)
        result = expand_char_ngrams([['xy', 'x']], 1, 1)
        result = tf.sparse.to_dense(result, default_value='')

        expected, result = self.evaluate(expected), self.evaluate(result)
        self.assertAllEqual(expected, result)

    def testSparse(self):
        source = tf.string_split(['ab|c d|e', 'f|'], delimiter='|')
        result = expand_char_ngrams(source, 1, 2)
        result = tf.sparse.to_dense(result, default_value='')
        expected = tf.convert_to_tensor([
            [
                ['a', 'b', 'ab', '', ''],
                ['c', ' ', 'd', 'c ', ' d'],
                ['e', '', '', '', '']
            ],
            [
                ['f', '', '', '', ''],
                ['', '', '', '', ''],
                ['', '', '', '', ''],
            ]
        ], dtype=tf.string)

        expected, result = self.evaluate(expected), self.evaluate(result)
        self.assertAllEqual(expected.tolist(), result.tolist())

    def testNone(self):
        result = expand_char_ngrams('123', 4, 5)
        result = tf.sparse.to_dense(result, default_value='')

        result = self.evaluate(result)
        self.assertAllEqual([], result.tolist())

    def testAsIsBelow(self):
        result = expand_char_ngrams('1234', 2, 3)
        result = tf.sparse.to_dense(result, default_value='')

        result = self.evaluate(result)
        self.assertAllEqual([b'12', b'23', b'34', b'123', b'234'], result.tolist())

    def testAsIsInside(self):
        result = expand_char_ngrams('123', 2, 3)
        result = tf.sparse.to_dense(result, default_value='')

        result = self.evaluate(result)
        self.assertAllEqual([b'12', b'23', b'123'], result.tolist())

    def testAsIsAbove(self):
        result = expand_char_ngrams('123', 4, 5)
        result = tf.sparse.to_dense(result, default_value='')

        result = self.evaluate(result)
        self.assertAllEqual([], result.tolist())

    def testNeverBelow(self):
        result = expand_char_ngrams('1234', 2, 3, itself='NEVER')
        result = tf.sparse.to_dense(result, default_value='')

        result = self.evaluate(result)
        self.assertAllEqual([b'12', b'23', b'34', b'123', b'234'], result.tolist())

    def testNeverInside(self):
        result = expand_char_ngrams('123', 2, 3, itself='NEVER')
        result = tf.sparse.to_dense(result, default_value='')

        result = self.evaluate(result)
        self.assertAllEqual([b'12', b'23'], result.tolist())

    def testNeverAbove(self):
        result = expand_char_ngrams('123', 4, 5, itself='NEVER')
        result = tf.sparse.to_dense(result, default_value='')

        result = self.evaluate(result)
        self.assertAllEqual([], result.tolist())

    def testAlwaysBelow(self):
        result = expand_char_ngrams('1234', 2, 3, itself='ALWAYS')
        result = tf.sparse.to_dense(result, default_value='')

        result = self.evaluate(result)
        self.assertAllEqual([b'12', b'23', b'34', b'123', b'234', b'1234'], result.tolist())

    def testAlwaysInside(self):
        result = expand_char_ngrams('123', 2, 3, itself='ALWAYS')
        result = tf.sparse.to_dense(result, default_value='')

        result = self.evaluate(result)
        self.assertAllEqual([b'12', b'23', b'123'], result.tolist())

    def testAlwaysAbove(self):
        result = expand_char_ngrams('123', 4, 5, itself='ALWAYS')
        result = tf.sparse.to_dense(result, default_value='')

        result = self.evaluate(result)
        self.assertAllEqual([b'123'], result.tolist())

    def testAloneBelow(self):
        result = expand_char_ngrams('1234', 2, 3, itself='ALONE')
        result = tf.sparse.to_dense(result, default_value='')

        result = self.evaluate(result)
        self.assertAllEqual([b'12', b'23', b'34', b'123', b'234'], result.tolist())

    def testAloneInside(self):
        result = expand_char_ngrams('123', 2, 3, itself='ALONE')
        result = tf.sparse.to_dense(result, default_value='')

        result = self.evaluate(result)
        self.assertAllEqual([b'12', b'23'], result.tolist())

    def testAloneAbove(self):
        result = expand_char_ngrams('123', 4, 5, itself='ALONE')
        result = tf.sparse.to_dense(result, default_value='')

        result = self.evaluate(result)
        self.assertAllEqual([b'123'], result.tolist())


if __name__ == "__main__":
    tf.test.main()
