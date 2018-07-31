from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from future.standard_library import install_aliases
install_aliases()
from urllib.request import urlopen

EMOJI_DATA_URL = 'http://www.unicode.org/Public/emoji/latest/emoji-data.txt'
PROPERTIES_DATA_URL = 'https://www.unicode.org/Public/UCD/latest/ucd/DerivedCoreProperties.txt'
SCRIPT_DATA_URL = 'https://www.unicode.org/Public/UCD/latest/ucd/Scripts.txt'
PROPSLIST_DATA_URL = 'https://www.unicode.org/Public/UCD/latest/ucd/PropList.txt'
EASTASIAN_DATA_URL = 'https://www.unicode.org/Public/UCD/latest/ucd/EastAsianWidth.txt'

GEN_TEMPLATE = """
  bool _gen_is_{}(char32_t c) {{
    return {};
  }}"""


def gen_func(data_url, block_name, func_title, skip=None):
    codes = []

    for line in urlopen(data_url).readlines():
        line = line.decode('utf-8')
        if line.startswith('#'):
            continue
        if not ('; {}#'.format(block_name) in line or '; {} '.format(block_name) in line
                or ';{}#'.format(block_name) in line or ';{} '.format(block_name) in line):
            continue
        if skip is not None and skip(line):
            continue

        codes.append(line.split(';')[0].strip())
    codes = sorted(codes, key=code_cardinality, reverse=True)

    conditions = [code_condition(c) for c in codes]
    conditions = ' || '.join(conditions)

    return GEN_TEMPLATE.format(func_title, conditions)


def code_cardinality(x):
    score = 1
    if '..' in x:
        begin, end = x.split('..')
        score = int(end, 16) - int(begin, 16) - 1

    return score


def code_condition(code):
    if '..' in code:
        range_begin, range_end = code.split('..')
        begin_point, end_point = code_point(range_begin), code_point(range_end)
        condition = '({} <= c && {} >= c)'.format(begin_point, end_point)

        begin_value, end_value = int(range_begin, 16), int(range_end, 16)
        if end_value - begin_value == 1:
            condition = '{} == c || {} == c'.format(begin_point, end_point)
    else:
        point = code_point(code)
        condition = '{} == c'.format(point)

    return condition


def code_point(code):
    zeros = '0' * (4 - len(code))
    template = "U'\\u{}{}'"

    if len(code) > 4:
        zeros = '0' * (8 - len(code))
        template = "U'\\U{}{}'"

    return template.format(zeros, code)


print('Auto-generated functions to use within expand_split_words op:\n')
print(gen_func(EMOJI_DATA_URL, 'Extended_Pictographic', 'extended_pictographic'))
print(gen_func(
    PROPERTIES_DATA_URL, 'Grapheme_Extend', 'other_grapheme_extend', skip=lambda l: '# Me' in l or '# Mn' in l))
print(gen_func(PROPERTIES_DATA_URL, 'Alphabetic', 'alphabetic'))
print(gen_func(SCRIPT_DATA_URL, 'Katakana', 'script_katakana'))
print(gen_func(SCRIPT_DATA_URL, 'Hiragana', 'script_hiragana'))
print(gen_func(SCRIPT_DATA_URL, 'Hebrew', 'script_hebrew'))
print(gen_func(PROPSLIST_DATA_URL, 'Ideographic', 'ideographic'))
print(gen_func(EASTASIAN_DATA_URL, 'F', 'east_asian_width_full'))
print('\n')
