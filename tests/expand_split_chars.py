# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
from tfucops import expand_split_chars


class SplitCharsTest(tf.test.TestCase):
    def testShape(self):
        with self.test_session():
            result = tf.shape(expand_split_chars([
                ['1', '2', '3'],
                ['4', '5', '6'],
            ])).eval()
            self.assertAllEqual([2, 3, 1], result)

    def test0D(self):
        with self.test_session():
            expected = tf.convert_to_tensor(['x', 'y'], dtype=tf.string).eval()
            result = expand_split_chars('xy').eval()
            self.assertAllEqual(expected, result)

    def test1D(self):
        with self.test_session():
            expected = tf.convert_to_tensor([['x', 'y']], dtype=tf.string).eval()
            result = expand_split_chars(['xy']).eval()
            self.assertAllEqual(expected, result)

    def test2D(self):
        with self.test_session():
            expected = tf.convert_to_tensor([[['x', 'y']]], dtype=tf.string).eval()
            result = expand_split_chars([['xy']]).eval()
            self.assertAllEqual(expected, result)

    def testEmpty(self):
        with self.test_session():
            expected = tf.convert_to_tensor([''], dtype=tf.string).eval()
            result = expand_split_chars('').eval()
            self.assertAllEqual(expected, result)

    def testRestore(self):
        source = 'Hey\n\tthere\t«word», !!!'

        result_op = expand_split_chars(source)
        result_op = tf.reduce_join(result_op)

        with self.test_session():
            expected = tf.convert_to_tensor(source, dtype=tf.string).eval()
            result = result_op.eval()
            self.assertAllEqual(expected, result)


if __name__ == "__main__":
    tf.test.main()
