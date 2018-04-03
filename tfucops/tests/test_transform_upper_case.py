# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
from .. import transform_upper_case


class TransformUppercaseUnicodeTest(tf.test.TestCase):
    def testInferenceShape(self):
        source = [
            ['1', '2', '3'],
            ['4', '5', '6'],
        ]
        result = transform_upper_case(source)

        self.assertEqual([2, 3], result.shape.as_list())

    def testActualShape(self):
        source = [
            ['1', '2', '3'],
            ['4', '5', '6'],
        ]
        result = transform_upper_case(source)
        result = tf.shape(result)

        with self.test_session():
            result = result.eval()
            self.assertEqual([2, 3], result.tolist())

    def testEmpty(self):
        result = transform_upper_case('')

        with self.test_session():
            result = result.eval()
            self.assertAllEqual(b'', result)

    def test0D(self):
        result = transform_upper_case('x')

        with self.test_session():
            result = result.eval()
            self.assertAllEqual(b'X', result)

    def test1D(self):
        result = transform_upper_case(['x'])

        with self.test_session():
            result = result.eval()
            self.assertAllEqual([b'X'], result)

    def test2D(self):
        result = transform_upper_case([['x']])

        with self.test_session():
            result = result.eval()
            self.assertAllEqual([[b'X']], result)

    def testLatin(self):
        result = transform_upper_case('TeSt')

        with self.test_session():
            result = result.eval()
            self.assertAllEqual(b'TEST', result)

    def testUnicode(self):
        expected = u'ТЕСТ'
        result = transform_upper_case(u'ТеСт')
        expected = tf.convert_to_tensor(expected, dtype=tf.string)

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected, result)
