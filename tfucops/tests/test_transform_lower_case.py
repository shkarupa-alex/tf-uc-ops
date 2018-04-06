# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
from .. import transform_lower_case


class TransformLowercaseUnicodeTest(tf.test.TestCase):
    def testInferenceShape(self):
        source = [
            ['1', '2', '3'],
            ['4', '5', '6'],
        ]
        result = transform_lower_case(source)

        self.assertEqual([2, 3], result.shape.as_list())

    def testActualShape(self):
        source = [
            ['1', '2', '3'],
            ['4', '5', '6'],
        ]
        result = transform_lower_case(source)
        result = tf.shape(result)

        with self.test_session():
            result = result.eval()
            self.assertEqual([2, 3], result.tolist())

    def testEmpty(self):
        result = transform_lower_case('')

        with self.test_session():
            result = result.eval()
            self.assertAllEqual(b'', result)

    def test0D(self):
        result = transform_lower_case('X')

        with self.test_session():
            result = result.eval()
            self.assertAllEqual(b'x', result)

    def test1D(self):
        result = transform_lower_case(['X'])

        with self.test_session():
            result = result.eval()
            self.assertAllEqual([b'x'], result)

    def test2D(self):
        result = transform_lower_case([['X']])

        with self.test_session():
            result = result.eval()
            self.assertAllEqual([[b'x']], result)

    def testSparse(self):
        expected = tf.SparseTensor(indices=[[0, 0]], values=[b'x'], dense_shape=[1, 1])
        source = tf.SparseTensor(indices=[[0, 0]], values=[b'X'], dense_shape=[1, 1])
        result = transform_lower_case(source)

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected.indices.tolist(), result.indices.tolist())
            self.assertAllEqual(expected.values.tolist(), result.values.tolist())
            self.assertAllEqual(expected.dense_shape.tolist(), result.dense_shape.tolist())

    def testUnicode(self):
        expected = tf.convert_to_tensor(u'тест', dtype=tf.string)
        result = transform_lower_case(u'ТеСт')

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected, result)
