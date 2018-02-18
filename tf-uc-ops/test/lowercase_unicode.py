# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
from ..lib import lowercase_unicode


class LowercaseUnicodeTest(tf.test.TestCase):
    def test0D(self):
        with self.test_session():
            result = lowercase_unicode('').eval()
            self.assertAllEqual('', result)

    def test1D(self):
        with self.test_session():
            result = lowercase_unicode(['']).eval()
            self.assertAllEqual([''], result)

    def test2D(self):
        with self.test_session():
            result = lowercase_unicode([['']]).eval()
            self.assertAllEqual([['']], result)

    def testLatin(self):
        expected = 'test'

        with self.test_session():
            result = lowercase_unicode('TeSt').eval()
            self.assertAllEqual(expected, result)

    def testCyrillic(self):
        expected = u'тест'.encode('utf-8')

        with self.test_session():
            result = lowercase_unicode(u'ТеСт').eval()
            self.assertAllEqual(expected, result)


if __name__ == "__main__":
    tf.test.main()
