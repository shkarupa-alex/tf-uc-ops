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

    def testDefault2D(self):
        with self.test_session():
            expected = tf.convert_to_tensor([[
                ['x', 'y'],
                ['x', 'D']
            ]], dtype=tf.string).eval()
            result = expand_char_ngrams([['xy', 'x']], 1, 1, default='D').eval()
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

    def testAsIsBelow(self):
        with self.test_session():
            result = expand_char_ngrams('1234', 2, 3).eval()
            self.assertAllEqual([b'12', b'23', b'34', b'123', b'234'], result.tolist())

    def testAsIsInside(self):
        with self.test_session():
            result = expand_char_ngrams('123', 2, 3).eval()
            self.assertAllEqual([b'12', b'23', b'123'], result.tolist())

    def testAsIsAbove(self):
        with self.test_session():
            result = expand_char_ngrams('123', 4, 5).eval()
            self.assertAllEqual([b''], result.tolist())

    def testNeverBelow(self):
        with self.test_session():
            result = expand_char_ngrams('1234', 2, 3, itself='NEVER').eval()
            self.assertAllEqual([b'12', b'23', b'34', b'123', b'234'], result.tolist())

    def testNeverInside(self):
        with self.test_session():
            result = expand_char_ngrams('123', 2, 3, itself='NEVER').eval()
            self.assertAllEqual([b'12', b'23'], result.tolist())

    def testNeverAbove(self):
        with self.test_session():
            result = expand_char_ngrams('123', 4, 5, itself='NEVER').eval()
            self.assertAllEqual([b''], result.tolist())

    def testAlwaysBelow(self):
        with self.test_session():
            result = expand_char_ngrams('1234', 2, 3, itself='ALWAYS').eval()
            self.assertAllEqual([b'12', b'23', b'34', b'123', b'234', b'1234'], result.tolist())

    def testAlwaysInside(self):
        with self.test_session():
            result = expand_char_ngrams('123', 2, 3, itself='ALWAYS').eval()
            self.assertAllEqual([b'12', b'23', b'123'], result.tolist())

    def testAlwaysAbove(self):
        with self.test_session():
            result = expand_char_ngrams('123', 4, 5, itself='ALWAYS').eval()
            self.assertAllEqual([b'123'], result.tolist())
