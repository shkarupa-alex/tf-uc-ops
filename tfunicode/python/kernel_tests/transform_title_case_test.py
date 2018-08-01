# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from tfunicode.python.ops import transform_title_case
import tensorflow as tf


class TransformTitlecaseUnicodeTest(tf.test.TestCase):
    def testInferenceShape(self):
        source = [
            ['1', '2', '3'],
            ['4', '5', '6'],
        ]
        result = transform_title_case(source)

        self.assertEqual([2, 3], result.shape.as_list())

    def testActualShape(self):
        source = [
            ['1', '2', '3'],
            ['4', '5', '6'],
        ]
        result = transform_title_case(source)
        result = tf.shape(result)

        with self.test_session():
            result = result.eval()
            self.assertEqual([2, 3], result.tolist())

    def testEmpty(self):
        result = transform_title_case('')

        with self.test_session():
            result = result.eval()
            self.assertAllEqual(b'', result)

    def test0D(self):
        result = transform_title_case('x')

        with self.test_session():
            result = result.eval()
            self.assertAllEqual(b'X', result)

    def test1D(self):
        result = transform_title_case(['x'])

        with self.test_session():
            result = result.eval()
            self.assertAllEqual([b'X'], result)

    def test2D(self):
        result = transform_title_case([['x']])

        with self.test_session():
            result = result.eval()
            self.assertAllEqual([[b'X']], result)

    def testSparse(self):
        expected = tf.SparseTensor(indices=[[0, 0]], values=[b'X'], dense_shape=[1, 1])
        source = tf.SparseTensor(indices=[[0, 0]], values=[b'x'], dense_shape=[1, 1])
        result = transform_title_case(source)

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected.indices.tolist(), result.indices.tolist())
            self.assertAllEqual(expected.values.tolist(), result.values.tolist())
            self.assertAllEqual(expected.dense_shape.tolist(), result.dense_shape.tolist())

    def testLatin(self):
        result = transform_title_case('TeSt')

        with self.test_session():
            result = result.eval()
            self.assertAllEqual(b'Test', result)

    def testUnicode(self):
        expected = [u'Тест', u'\u01C5']
        result = transform_title_case([u'Тест', u'\u01C6'])
        expected = tf.convert_to_tensor(expected, dtype=tf.string)

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected, result)


if __name__ == "__main__":
    tf.test.main()
