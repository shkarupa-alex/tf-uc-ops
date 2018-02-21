# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf

from expand_split_chars import SplitCharsTest
from expand_split_words import SplitWordsTest
from transform_lower_case import LowercaseUnicodeTest
from transform_normalize_unicode import NormalizeUnicodeTest


if __name__ == "__main__":
    tf.test.main()
