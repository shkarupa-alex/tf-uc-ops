# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
from tfucops import transform_lower_case


class LowercaseUnicodeTest(tf.test.TestCase):
    def test0D(self):
        with self.test_session():
            result = transform_lower_case('X').eval()
            self.assertAllEqual(b'x', result)

    def test1D(self):
        with self.test_session():
            result = transform_lower_case(['X']).eval()
            self.assertAllEqual([b'x'], result)

    def test2D(self):
        with self.test_session():
            result = transform_lower_case([['X']]).eval()
            self.assertAllEqual([[b'x']], result)

    def testLatin(self):
        with self.test_session():
            result = transform_lower_case('TeSt').eval()
            self.assertAllEqual(b'test', result)

    def testCyrillic(self):
        expected = u'тест'.encode('utf-8')

        with self.test_session():
            result = transform_lower_case(u'ТеСт').eval()
            self.assertAllEqual(expected, result)
