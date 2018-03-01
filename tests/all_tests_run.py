# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf

from expand_split_chars import ExpandSplitCharsTest
from expand_split_words import ExpandSplitWordsTest
from transform_lower_case import TransformLowercaseUnicodeTest
from transform_normalize_unicode import TransformNormalizeUnicodeTest
from transform_zero_digits import TransformZeroDigitsTest


if __name__ == "__main__":
    tf.test.main()
