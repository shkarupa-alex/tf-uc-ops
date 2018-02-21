# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
from tfucops import expand_split_words


class SplitWordsTest(tf.test.TestCase):
    def testShape(self):
        with self.test_session():
            result = tf.shape(expand_split_words([
                ['1', '2', '3'],
                ['4', '5', '6'],
            ])).eval()
            self.assertAllEqual([2, 3, 1], result)

    def test0D(self):
        with self.test_session():
            expected = tf.convert_to_tensor(['x', '!'], dtype=tf.string).eval()
            result = expand_split_words('x!').eval()
            self.assertAllEqual(expected, result)

    def test1D(self):
        with self.test_session():
            expected = tf.convert_to_tensor([['x', '!']], dtype=tf.string).eval()
            result = expand_split_words(['x!']).eval()
            self.assertAllEqual(expected, result)

    def test2D(self):
        with self.test_session():
            expected = tf.convert_to_tensor([[['x', '!']]], dtype=tf.string).eval()
            result = expand_split_words([['x!']]).eval()
            self.assertAllEqual(expected, result)

    def testEmpty(self):
        with self.test_session():
            expected = tf.convert_to_tensor([''], dtype=tf.string).eval()
            result = expand_split_words('').eval()
            self.assertAllEqual(expected, result)

    def testRestore(self):
        source = u'Hey\n\tthere\t«word», !!!'

        result_op = expand_split_words(source)
        result_op = tf.reduce_join(result_op)

        with self.test_session():
            result = result_op.eval()
            expected = tf.convert_to_tensor(source, dtype=tf.string).eval()
            self.assertAllEqual(expected, result)

    def testWrapped(self):
        result_op = expand_split_words([
            ' "word" ',
            u' «word» ',
            u' „word“ ',
            ' {word} ',
            ' (word) ',
            ' [word] ',
            ' <word> ',
        ])
        expected = [
            [' ', '"', 'word', '"', ' '],
            [' ', u'«', 'word', u'»', ' '],
            [' ', u'„', 'word', u'“', ' '],
            [' ', '{', 'word', '}', ' '],
            [' ', '(', 'word', ')', ' '],
            [' ', '[', 'word', ']', ' '],
            [' ', '<', 'word', '>', ' '],
        ]

        with self.test_session():
            result = result_op.eval()
            expected = tf.convert_to_tensor(expected, dtype=tf.string).eval()
            self.assertAllEqual(expected, result)

    def testWordPunkt(self):
        result_op = expand_split_words([
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

        with self.test_session():
            result = result_op.eval()
            expected = tf.convert_to_tensor(expected, dtype=tf.string).eval()
            self.assertAllEqual(expected, result)

    def testComplexWord(self):
        result_op = expand_split_words([
            ' test@test.com ',
            ' www.test.com ',
            ' word..word ',
            ' word+word-word ',
            ' word\\word/word#word ',
        ])
        expected = [
            [' ', 'test', '@', 'test.com', ' ', '', '', '', ''],
            [' ', 'www.test.com', ' ', '', '', '', '', '', ''],
            [' ', 'word', '.', '.', 'word', ' ', '', '', ''],
            [' ', 'word', '+', 'word', '-', 'word', ' ', '', ''],
            [' ', 'word', '\\', 'word', '/', 'word', '#', 'word', ' '],
        ]

        with self.test_session():
            result = result_op.eval()
            expected = tf.convert_to_tensor(expected, dtype=tf.string).eval()
            self.assertAllEqual(expected, result)


if __name__ == "__main__":
    tf.test.main()
