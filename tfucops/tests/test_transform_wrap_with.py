# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
from .. import transform_wrap_with


class TransformWrapWithTest(tf.test.TestCase):
    def test0D(self):
        with self.test_session():
            result = transform_wrap_with('X', '<', '>').eval()
            self.assertAllEqual(b'<X>', result)

    def test1D(self):
        with self.test_session():
            result = transform_wrap_with(['X'], '<', '>').eval()
            self.assertAllEqual([b'<X>'], result)

    def test2D(self):
        with self.test_session():
            result = transform_wrap_with([['X']], '<', '>').eval()
            self.assertAllEqual([[b'<X>']], result)

    def testEmptySource(self):
        with self.test_session():
            result = transform_wrap_with('', '<', '>').eval()
            self.assertAllEqual(b'<>', result)

    def testEmptyBorders(self):
        with self.test_session():
            result = transform_wrap_with('test', '', '').eval()
            self.assertAllEqual(b'test', result)

    def testCyrillic(self):
        expected = u'надо'

        with self.test_session():
            result = transform_wrap_with(u'ад', u'н', u'о').eval()
            expected = tf.convert_to_tensor(expected, dtype=tf.string).eval()
            self.assertAllEqual(expected, result)

