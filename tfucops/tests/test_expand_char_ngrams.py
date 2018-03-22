# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
from .. import expand_char_ngrams


class ExpandCharNgramsTest(tf.test.TestCase):
    def testShape(self):
        with self.test_session():
            result = tf.shape(expand_char_ngrams([
                ['1', '2', '3'],
                ['4', '5', '6'],
            ], 1, 1)).eval()
            self.assertAllEqual([2, 3, 1], result)

    def test0D(self):
        with self.test_session():
            expected = tf.convert_to_tensor(['x', 'y'], dtype=tf.string).eval()
            result = expand_char_ngrams('xy', 1, 1).eval()
            self.assertAllEqual(expected, result)

    def test1D(self):
        with self.test_session():
            expected = tf.convert_to_tensor([['x', 'y']], dtype=tf.string).eval()
            result = expand_char_ngrams(['xy'], 1, 1).eval()
            self.assertAllEqual(expected, result)

    def test2D(self):
        with self.test_session():
            expected = tf.convert_to_tensor([[['x', 'y']]], dtype=tf.string).eval()
            result = expand_char_ngrams([['xy']], 1, 1).eval()
            self.assertAllEqual(expected, result)

    def testEmpty(self):
        with self.test_session():
            expected = tf.convert_to_tensor([''], dtype=tf.string).eval()
            result = expand_char_ngrams('', 1, 1).eval()
            self.assertAllEqual(expected, result)

    def testNone(self):
        with self.test_session():
            result = expand_char_ngrams('123', 4, 5).eval()
            self.assertAllEqual([b''], result.tolist())

    def testNormal(self):
        with self.test_session():
            result = expand_char_ngrams('1234', 2, 3).eval()
            self.assertAllEqual([
                b'12', b'23', b'34',
                b'123', b'234'
            ], result.tolist())
