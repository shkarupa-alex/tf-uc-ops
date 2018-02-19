from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
from .transform_normalize_unicode import NormalizeUnicodeTest
from .transform_lower_case import LowercaseUnicodeTest
from .expand_split_words import SplitWordsTest

if __name__ == "__main__":
    tf.test.main()
