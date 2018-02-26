# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
from tfucops import transform_zero_digits


class TransformZeroDigitsTest(tf.test.TestCase):
    def test0D(self):
        with self.test_session():
            result = transform_zero_digits('7').eval()
            self.assertAllEqual(b'0', result)

    def test1D(self):
        with self.test_session():
            result = transform_zero_digits(['7']).eval()
            self.assertAllEqual([b'0'], result)

    def test2D(self):
        with self.test_session():
            result = transform_zero_digits([['7']]).eval()
            self.assertAllEqual([[b'0']], result)

    def testPlain(self):
        with self.test_session():
            result = transform_zero_digits('123456789').eval()
            self.assertAllEqual(b'000000000', result)

    def testMixed(self):
        with self.test_session():
            result = transform_zero_digits(u'P.1, АБ1, ЯК12x').eval()
            expected = tf.convert_to_tensor(u'P.0, АБ0, ЯК00x', dtype=tf.string).eval()
            self.assertAllEqual(expected, result)


if __name__ == "__main__":
    tf.test.main()
