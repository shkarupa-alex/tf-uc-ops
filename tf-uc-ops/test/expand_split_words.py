# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
from ..ops import expand_split_words


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
        source = 'Hey\n\tthere\t«word», !!!'

        result_op = expand_split_words(source)
        result_op = tf.reduce_join(result_op, 0)

        with self.test_session():
            result = result_op.eval()
            self.assertAllEqual(source, result)

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
        expected = [[__.encode('utf-8') for __ in _] for _ in expected]

        with self.test_session():
            result = result_op.eval()
            self.assertAllEqual(expected, result)

            # def testWordPunkt(self):
            #     result_op = expand_split_words([
            #         ' word. ',
            #         ' word.. ',
            #         ' word... ',
            #         u' word… '
            #         ' word, ',
            #         ' word., ',
            #         ' word: ',
            #         ' word; ',
            #         ' word! ',
            #         ' word? ',
            #         ' word% ',
            #         ' $word ',
            #     ])
            #     expected_values = [
            #         ' ', 'word', '.', ' ',
            #         ' ', 'word', '.', '.', ' ',
            #         ' ', 'word', '.', '.', '.', ' ',
            #         ' ', 'word', u'…', ' ',
            #         ' ', 'word', ',', ' ',
            #         ' ', 'word', '.', ',', ' ',
            #         ' ', 'word', ':', ' ',
            #         ' ', 'word', ';', ' ',
            #         ' ', 'word', '!', ' ',
            #         ' ', 'word', '?', ' ',
            #         ' ', 'word', '%', ' ',
            #         ' ', '$', 'word', ' '
            #     ]
            #     expected_values = [_.encode('utf-8') for _ in expected_values]
            #
            #     with self.test_session():
            #         result = result_op.eval()
            #         self.assertAllEqual(expected_values, result.values)
            #
            # def testComplexWord(self):
            #     result_op = expand_split_words([
            #         ' test@test.com ',
            #         ' www.test.com ',
            #         ' word..word ',
            #         ' word+word-word ',
            #         ' word\\word/word#word ',
            #     ])
            #     expected_values = [
            #         ' ', 'test', '@', 'test', '.', 'com', ' ',
            #         ' ', 'www', '.', 'test', '.', 'com', ' ',
            #         ' ', 'word', '.', '.', 'word', ' ',
            #         ' ', 'word', '+', 'word', '-', 'word', ' ',
            #         ' ', 'word', '\\', 'word', '/', 'word', '#', 'word', ' '
            #     ]
            #     expected_values = [_.encode('utf-8') for _ in expected_values]
            #
            #     with self.test_session():
            #         result = result_op.eval()
            #         print(result)
            #         # self.assertAllEqual(expected_values, result.values)
            #
            # def testCharBreaks(self):
            #     result_op = expand_split_words([
            #         u' а_также ',
            #         u' т.д. ',
            #         u' José ',
            #         u' ЁёЁёй ',
            #         ' 1,5 ',
            #         ' 01.01.2018 ',
            #     ])
            #     expected_values = [
            #         ' ', u'а', '_', u'также', ' ',
            #         ' ', u'т', '.', u'д', '.', ' ',
            #         ' ', u'José', ' ',
            #         ' ', u'ЁёЁёй', ' ',
            #         ' ', '1', ',', '5', ' ',
            #         ' ', '01', '.', '01', '.', '2018', ' '
            #     ]
            #     expected_values = [_.encode('utf-8') for _ in expected_values]
            #
            #     with self.test_session():
            #         result = result_op.eval()
            #         # self.assertAllEqual(expected_values, result.values)


if __name__ == "__main__":
    tf.test.main()
