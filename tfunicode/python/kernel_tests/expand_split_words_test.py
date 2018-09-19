# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from future.standard_library import install_aliases

install_aliases()
from tfunicode.python.ops import expand_split_words
from urllib.request import urlopen
import os
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

    def testIcuWordBreak(self):
        # WORD_BREAK_TEST_URL = 'https://www.unicode.org/Public/UCD/latest/ucd/auxiliary/WordBreakTest.txt'
        # test_data = urlopen(WORD_BREAK_TEST_URL).read().decode('utf-8').strip().split('\n')

        WORD_BREAK_TEST_FILE = os.path.join(os.path.dirname(__file__), 'WordBreakTest.txt')
        with open(WORD_BREAK_TEST_FILE, 'rb') as ft:
            test_data = ft.read().decode('utf-8').strip().split('\n')

        expected, source, description = [], [], []
        for row, line in enumerate(test_data):
            if line.startswith('#'):
                continue

            example, rule = line.split('#')
            example = example.strip().strip(u'÷').strip().replace(u'÷', '00F7').replace(u'×', '00D7').split(' ')
            example = [code.zfill(8) if len(code) > 4 else code.zfill(4) for code in example]
            example = [u'\\U{}'.format(code) if len(code) > 4 else u'\\u{}'.format(code) for code in example]
            example = [code.decode('unicode-escape') for code in example]
            example = u''.join(example).replace(u'×', '')

            expected.append(example.split(u'÷'))
            source.append(example.replace(u'÷', ''))

            rule = rule.strip().strip(u'÷').strip()
            description.append(u'Row #{}. {}'.format(row + 1, rule))

        max_len = len(sorted(expected, key=len, reverse=True)[0])
        expected = [e + ['']*(max_len - len(e)) for e in expected]

        expected_tensor = tf.convert_to_tensor(expected, dtype=tf.string)
        result_tensor = tf.sparse_tensor_to_dense(expand_split_words(source), default_value='')

        with self.test_session():
            expected_value, result_value = expected_tensor.eval(), result_tensor.eval()

        for exp, res, desc in zip(expected_value, result_value, description):
            self.assertAllEqual(exp, res, desc)

    # def testAdditionalSplit(self):
    #     expected = [
    #         # [' ', 'test', '@', 'test.com', ' ', '', '', '', ''],
    #         # [' ', 'www.test.com', ' ', '', '', '', '', '', ''],
    #         # [' ', 'word', '.', '.', 'word', ' ', '', '', ''],
    #         # [' ', 'word', '+', 'word', '-', 'word', ' ', '', ''],
    #         # [' ', 'word', '\\', 'word', '/', 'word', '#', 'word', ' '],
    #     ]
    #     expected = tf.convert_to_tensor(expected, dtype=tf.string)
    #     result = expand_split_words([
    #         # 46
    #         u' 29.03.2009. ',
    #         u' U.S. ',
    #         u' Яндекс.Диск ',
    #         u' 12.Main ',
    #         u' kw.9 ',
    #
    #         u' https://wikidata.org/wiki/Track:Q256507 ',
    #         u' 1,5 ',
    #         u' Категория:Родившиеся ',
    #         u' поселе́ние ',
    #         u' Don’t ',
    #         u' Don\'t ',
    #         u' кВт·ч ',
    #         u' 1⁄3 ',
    #         u' ар\u00ADтель ',
    #         u' минэ̀м ',
    #         u' Rock‘s ',
    #         u' o̯a ',
    #         u' D̲h̲u ',
    #         u' d͡ʒ ',
    #         u' са̄мь ',
    #         u' t͜s ',
    #         u' 23;0 ',
    #         u' Sofii︠a︡ ',
    #         u' издательс︣тво ',
    #         u' kn̡iga ',
    #         u' sɐ̃ũ ',
    #         u' kʰa̰ɴ ',
    #         u' є҆ди́но ',
    #         u' side： ',
    #         u' zir̂ks ',
    #         u' бж҃е ',
    #         u' црⷭ҇тва ',
    #         u' гдⷭ҇и ',
    #         u'  ',
    #         u'  ',
    #         u'  ',
    #         u'  ',
    #         u'  ',
    #         u'  ',
    #     ])
    #     result = tf.sparse_tensor_to_dense(result, default_value='')
    #
    #     with self.test_session():
    #         expected, result = expected.eval(), result.eval()
    #         self.assertAllEqual(expected, result)

if __name__ == "__main__":
    tf.test.main()
