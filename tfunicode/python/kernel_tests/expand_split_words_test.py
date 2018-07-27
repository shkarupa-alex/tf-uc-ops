# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from tfunicode.python.ops import expand_split_words
import tensorflow as tf


class ExpandSplitWordsTest(tf.test.TestCase):
    def testInferenceShape(self):
        source = [
            ['1', '2', '3'],
            ['4', '5', '6'],
        ]
        result = expand_split_words(source)

        self.assertEqual([None, 3], result.indices.shape.as_list())
        self.assertEqual([None], result.values.shape.as_list())
        self.assertEqual([3], result.dense_shape.shape.as_list())

    def testActualShape(self):
        source = [
            ['1', '2', '3'],
            ['4', '5', '6'],
        ]
        result = expand_split_words(source)

        with self.test_session():
            result = result.eval()
            self.assertAllEqual([2, 3, 1], result.dense_shape)

    def testEmpty(self):
        expected = tf.convert_to_tensor([''], dtype=tf.string)
        result = expand_split_words('')
        result = tf.sparse_tensor_to_dense(result, default_value='')

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected, result)

    def test0D(self):
        expected = tf.convert_to_tensor(['x', '!'], dtype=tf.string)
        result = expand_split_words('x!')
        result = tf.sparse_tensor_to_dense(result, default_value='')

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected, result)

    def test1D(self):
        expected = tf.convert_to_tensor([['x', '!']], dtype=tf.string)
        result = expand_split_words(['x!'])
        result = tf.sparse_tensor_to_dense(result, default_value='')

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected, result)

    def test2D(self):
        expected = tf.convert_to_tensor([[['x', '!']]], dtype=tf.string)
        result = expand_split_words([['x!']])
        result = tf.sparse_tensor_to_dense(result, default_value='')

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected, result)

    def testSparse(self):
        source = tf.string_split(['ab|c d|e', 'f|'], delimiter='|')
        result = expand_split_words(source)
        result = tf.sparse_tensor_to_dense(result, default_value='')
        expected = tf.convert_to_tensor([
            [
                ['ab', '', ''],
                ['c', ' ', 'd'],
                ['e', '', '']
            ],
            [
                ['f', '', ''],
                ['', '', ''],
                ['', '', ''],
            ]
        ], dtype=tf.string)

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected.tolist(), result.tolist())

    def testRestore(self):
        source = u'Hey\n\tthere\t«word», !!!'
        expected = tf.convert_to_tensor(source, dtype=tf.string)
        result = expand_split_words(source)
        result = tf.sparse_tensor_to_dense(result, default_value='')
        result = tf.reduce_join(result)

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected, result)

    def testWrapped(self):
        expected = [
            [' ', '"', 'word', '"', ' '],
            [' ', u'«', 'word', u'»', ' '],
            [' ', u'„', 'word', u'“', ' '],
            [' ', '{', 'word', '}', ' '],
            [' ', '(', 'word', ')', ' '],
            [' ', '[', 'word', ']', ' '],
            [' ', '<', 'word', '>', ' '],
        ]
        expected = tf.convert_to_tensor(expected, dtype=tf.string)
        result = expand_split_words([
            ' "word" ',
            u' «word» ',
            u' „word“ ',
            ' {word} ',
            ' (word) ',
            ' [word] ',
            ' <word> ',
        ])
        result = tf.sparse_tensor_to_dense(result, default_value='')

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected, result)

    def testWordPunkt(self):
        expected = [
            [' ', 'word', '.', ' ', '', ''],
            [' ', 'word', '.', '.', ' ', ''],
            [' ', 'word', '.', '.', '.', ' '],
            [' ', 'word', u'…', ' ', '', ''],
            [' ', 'word', ',', ' ', '', ''],
            [' ', 'word', '.', ',', ' ', ''],
            [' ', 'word', ':', ' ', '', ''],
            [' ', 'word', ';', ' ', '', ''],
            [' ', 'word', '!', ' ', '', ''],
            [' ', 'word', '?', ' ', '', ''],
            [' ', 'word', '%', ' ', '', ''],
            [' ', '$', 'word', ' ', '', ''],
        ]
        expected = tf.convert_to_tensor(expected, dtype=tf.string)
        result = expand_split_words([
            ' word. ',
            ' word.. ',
            ' word... ',
            u' word… ',
            ' word, ',
            ' word., ',
            ' word: ',
            ' word; ',
            ' word! ',
            ' word? ',
            ' word% ',
            ' $word ',
        ])
        result = tf.sparse_tensor_to_dense(result, default_value='')

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected, result)

    def testComplexWord(self):
        expected = [
            [' ', 'test', '@', 'test.com', ' ', '', '', '', ''],
            [' ', 'www.test.com', ' ', '', '', '', '', '', ''],
            [' ', 'word', '.', '.', 'word', ' ', '', '', ''],
            [' ', 'word', '+', 'word', '-', 'word', ' ', '', ''],
            [' ', 'word', '\\', 'word', '/', 'word', '#', 'word', ' '],
        ]
        expected = tf.convert_to_tensor(expected, dtype=tf.string)
        result = expand_split_words([
            ' test@test.com ',
            ' www.test.com ',
            ' word..word ',
            ' word+word-word ',
            ' word\\word/word#word ',
        ])
        result = tf.sparse_tensor_to_dense(result, default_value='')

        with self.test_session():
            expected, result = expected.eval(), result.eval()
            self.assertAllEqual(expected, result)


if __name__ == "__main__":
    tf.test.main()
