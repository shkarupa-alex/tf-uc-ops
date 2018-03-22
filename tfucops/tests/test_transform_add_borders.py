# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
from .. import transform_add_borders


class TransformLowercaseUnicodeTest(tf.test.TestCase):
    def test0D(self):
        with self.test_session():
            result = transform_add_borders('X', '<', '>').eval()
            self.assertAllEqual(b'<X>', result)

    def test1D(self):
        with self.test_session():
            result = transform_add_borders(['X'], '<', '>').eval()
            self.assertAllEqual([b'<X>'], result)

    def test2D(self):
        with self.test_session():
            result = transform_add_borders([['X']], '<', '>').eval()
            self.assertAllEqual([[b'<X>']], result)

    def testEmptySource(self):
        with self.test_session():
            result = transform_add_borders('', '<', '>').eval()
            self.assertAllEqual(b'<>', result)

    def testEmptyBorders(self):
        with self.test_session():
            result = transform_add_borders('test', '', '').eval()
            self.assertAllEqual(b'test', result)

    def testCyrillic(self):
        expected = u'надо'

        with self.test_session():
            result = transform_add_borders(u'ад', u'н', u'о').eval()
            expected = tf.convert_to_tensor(expected, dtype=tf.string).eval()
            self.assertAllEqual(expected, result)

