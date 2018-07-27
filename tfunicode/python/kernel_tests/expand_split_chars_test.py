# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from tfunicode.python.ops import expand_split_chars
import tensorflow as tf


class ExpandSplitCharsTest(tf.test.TestCase):
    def testInferenceShape(self):
        source = [
            ['1', '2', '3'],
            ['4', '5', '6'],
        ]
        result = expand_split_chars(source)

        self.assertEqual([None, 3], result.indices.shape.as_list())
        self.assertEqual([None], result.values.shape.as_list())
        self.assertEqual([3], result.dense_shape.shape.as_list())

    def testActualShape(self):
        source = [
            ['1', '2', '3'],
            ['4', '5', '6'],
        ]
        result = expand_split_chars(source)

        with self.test_session():
            result = result.eval()
            self.assertAllEqual([2, 3, 1], result.dense_shape)

    def testEmpty(self):
        expected = tf.convert_to_tensor([], dtype=tf.string)
        result = expand_split_chars('')
        result = tf.sparse_tensor_to_dense(result, default_value='')

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected, result)

    def test0D(self):
        expected = tf.convert_to_tensor(['x', 'y'], dtype=tf.string)
        result = expand_split_chars('xy')
        result = tf.sparse_tensor_to_dense(result, default_value='')

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected, result)

    def test1D(self):
        expected = tf.convert_to_tensor([['x', 'y']], dtype=tf.string)
        result = expand_split_chars(['xy'])
        result = tf.sparse_tensor_to_dense(result, default_value='')

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected, result)

    def test2D(self):
        expected = tf.convert_to_tensor([[['x', 'y']]], dtype=tf.string)
        result = expand_split_chars([['xy']])
        result = tf.sparse_tensor_to_dense(result, default_value='')

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected, result)

    def testSparse(self):
        source = tf.string_split(['ab|c d|e', 'f|'], delimiter='|')
        result = expand_split_chars(source)
        result = tf.sparse_tensor_to_dense(result, default_value='')
        expected = tf.convert_to_tensor([
            [
                ['a', 'b', ''],
                ['c', ' ', 'd'],
                ['e', '', '']
            ],
            [
                ['f', '', ''],
                ['', '', ''],
                ['', '', ''],
            ]
        ], dtype=tf.string)

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected.tolist(), result.tolist())

    def testUnicode(self):
        expected = tf.convert_to_tensor([u'ё', u' ', u'е', u'̈', u'2', u'⁵'], dtype=tf.string)
        result = expand_split_chars(u'ё ё2⁵')
        result = tf.sparse_tensor_to_dense(result, default_value='')

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected, result)

    def testRestore(self):
        source = 'Hey\n\tthere\t«word», !!!'
        expected = tf.convert_to_tensor(source, dtype=tf.string)
        result = expand_split_chars(source)
        result = tf.sparse_tensor_to_dense(result, default_value='')
        result = tf.reduce_join(result)

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected, result)


if __name__ == "__main__":
    tf.test.main()
