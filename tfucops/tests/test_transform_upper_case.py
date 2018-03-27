# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
from .. import transform_upper_case


class TransformUppercaseUnicodeTest(tf.test.TestCase):
    def test0D(self):
        with self.test_session():
            result = transform_upper_case('x').eval()
            self.assertAllEqual(b'X', result)

    def test1D(self):
        with self.test_session():
            result = transform_upper_case(['x']).eval()
            self.assertAllEqual([b'X'], result)

    def test2D(self):
        with self.test_session():
            result = transform_upper_case([['x']]).eval()
            self.assertAllEqual([[b'X']], result)

    def testLatin(self):
        with self.test_session():
            result = transform_upper_case('TeSt').eval()
            self.assertAllEqual(b'TEST', result)

    def testCyrillic(self):
        expected = u'ТЕСТ'

        with self.test_session():
            result = transform_upper_case(u'ТеСт').eval()
            expected = tf.convert_to_tensor(expected, dtype=tf.string).eval()
            self.assertAllEqual(expected, result)

