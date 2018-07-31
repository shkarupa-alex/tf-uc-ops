#include "tfunicode/cc/lib/word_break.h"
#include "unilib/unicode.h"

using namespace ufal::unilib;
using namespace std;


// Break string into words with Unicode rules http://unicode.org/Public/UNIDATA/auxiliary/WordBreakTest.html#rules
// and their notations http://www.unicode.org/reports/tr29/#Word_Boundaries
// Returns true if character at position pos is a border of the word
bool WordBreak::IsBreak(const u32string &source, const int position) {
  const char32_t right_curr = CharAt(source, position);

  const int left_curr_pos = Skip_EFZ(source, position, -1, false);
  const char32_t left_curr = CharAt(source, left_curr_pos);

  // According to https://github.com/unicode-org/icu/blob/master/icu4c/source/data/brkitr/rules/word.txt
  // right after WB4 we should skip not only (Format | Extend) but also ZWJ.
  // As equivalent we use ZWJ for most left and right characters.

  const int left_left_pos = Skip_EFZ(source, left_curr_pos, -1, true);
  const char32_t left_left = CharAt(source, left_left_pos);

  const int right_right_pos = Skip_EFZ(source, position, +1, true);
  const char32_t right_right = CharAt(source, right_right_pos);

  // WB1
  if (1 > position) {
    return true;
  }

  // WB2
  if (source.length() <= position) {
    return true;
  }

  // WB3
  if (CR(left_curr)
    && LF(right_curr)) {
    return false;
  }

  // WB3a
  if (Newline(left_curr) || CR(left_curr) || LF(left_curr)) {
    return true;
  }

  // WB3b
  if (Newline(right_curr) || CR(right_curr) || LF(right_curr)) {
    return true;
  }

  // WB3c
  if (ZWJ(left_curr)
    && Extended_Pictographic(right_curr)) {
    return false;
  }

  // WB3d
  if (WSegSpace(left_curr)
    && WSegSpace(right_curr)) {
    return false;
  }

  // WB4
  if (
  !(1 > position || Newline(left_curr) || CR(left_curr) || LF(left_curr)) &&
  (Format(right_curr) || Extend(right_curr) || ZWJ(right_curr))) {
    return false;
  }

  // WB5
  if (AHLetter(left_curr)
    && AHLetter(right_curr)) {
    return false;
  }

  // WB6
  if (AHLetter(left_curr)
    && (MidLetter(right_curr) || MidNumLetQ(right_curr))
    && AHLetter(right_right)) {
    return false;
  }

  // WB7
  if (AHLetter(left_left)
    && (MidLetter(left_curr) || MidNumLetQ(left_curr))
    && AHLetter(right_curr)) {
    return false;
  }

  // WB7a
  if (Hebrew_Letter(left_curr)
    && Single_Quote(right_curr)) {
    return false;
  }

  // WB7b
  if (Hebrew_Letter(left_curr)
    && Double_Quote(right_curr)
    && Hebrew_Letter(right_right)) {
    return false;
  }

  // WB7c
  if (Hebrew_Letter(left_left)
    && Double_Quote(left_curr)
    && Hebrew_Letter(right_curr)) {
    return false;
  }

  // WB8
  if (Numeric(left_curr)
    && Numeric(right_curr)) {
    return false;
  }

  // WB9
  if (AHLetter(left_curr)
    && Numeric(right_curr)) {
    return false;
  }

  // WB10
  if (Numeric(left_curr)
    && AHLetter(right_curr)) {
    return false;
  }

  // WB11
  if (Numeric(left_left) &&
      (MidNum(left_curr) || MidNumLetQ(left_curr))
      && Numeric(right_curr)) {
    return false;
  }

  // WB12
  if (Numeric(left_curr) &&
      (MidNum(right_curr) || MidNumLetQ(right_curr))
      && Numeric(right_right)) {
    return false;
  }

  // WB13
  if (Katakana(left_curr) &&
      Katakana(right_curr)) {
    return false;
    }

  // WB13a
  if ((AHLetter(left_curr) || Numeric(left_curr) || Katakana(left_curr) || ExtendNumLet(left_curr))
    && ExtendNumLet(right_curr)) {
    return false;
  }

  // WB13b
  if (ExtendNumLet(left_curr) &&
      (AHLetter(right_curr) || Numeric(right_curr) || Katakana(right_curr))) {
    return false;
  }


  // WB15, WB16
  const int ri_left_curr_pos = Skip_EFZ(source, position, -1, true);
  const char32_t ri_left_curr = CharAt(source, ri_left_curr_pos);
  if (Regional_Indicator(ri_left_curr) && Regional_Indicator(right_curr)) {
    if (position <= 1) {
      return false;
    }

    int ri_before_pos = Skip_EFZ(source, ri_left_curr_pos, -1, true);
    int ri_before_count = 0;
    while (Regional_Indicator(source[ri_before_pos])) {
      ri_before_count++;
      ri_before_pos = Skip_EFZ(source, ri_before_pos, -1, true);
    }

    if (0 == ri_before_count % 2) {
      return false;
    }
  }

  return true;  // WB999
}

// Returns character at position or 0 if position invalid.
char32_t WordBreak::CharAt(const u32string &source, const int position) {
  if (0 > position || source.length() <= position) {
    return 0;
  }

  return source[position];
}

// Returns (-1 | length) if there are only (Extend | Format) in desired direction.
// Otherwise returns position of first !(Extend | Format) character.
int WordBreak::Skip_EFZ(const u32string &source, const int start, const int direction, const bool zwj) {

  if (0 == direction) {
    return start;
  }

  const int step = direction < 0 ? -1 : 1;
  int stop = start + step;

  while (-1 < stop && source.length() > stop
    && (Extend(source[stop]) || Format(source[stop]) || (zwj && ZWJ(source[stop])))) {
    stop += step;
  }

  return stop;
}

bool WordBreak::CR(char32_t c) {
  return U'\r' == c;
}

bool WordBreak::LF(char32_t c) {
  return U'\n' == c;
}

bool WordBreak::Newline(char32_t c) {
  return U'\u000B' == c || U'\u000C' == c || U'\u0085' == c || U'\u2028' == c || U'\u2029' == c;
}

bool WordBreak::Extend(char32_t c) {
  return (
      Grapheme_Extend(c)
      || (unicode::category(c) & unicode::Mc)  // General_Category = Spacing_Mark
      || Emoji_Modifier(c)
    )
    && !ZWJ(c);
}

bool WordBreak::ZWJ(char32_t c) {
  return U'\u200D' == c;
}

bool WordBreak::Regional_Indicator(char32_t c) {
  return U'\U0001F1E6' <= c && U'\U0001F1FF' >= c;
}

bool WordBreak::Format(char32_t c) {
  return
    (unicode::category(c) & unicode::Cf)  // General_Category = Format
    && !(U'\u200B' <= c && U'\u200D' >= c);
}

bool WordBreak::Katakana(char32_t c) {
  return
    _gen_is_script_katakana(c)
    || (U'\u3031' <= c && U'\u3035' >= c)
    || U'\u309B' == c || U'\u309C' == c || U'\u30A0' == c || U'\u30FC' == c || U'\uFF70' == c;
}

bool WordBreak::Hebrew_Letter(char32_t c) {
  return
    (unicode::category(c) & unicode::Lo)  // General_Category = Other_Letter
    && _gen_is_script_hebrew(c);
}

bool WordBreak::ALetter(char32_t c) {
  return (
      Alphabetic(c)
      || (U'\u02C2' <= c && U'\u02C5' >= c)
      || (U'\u02D2' <= c && U'\u02D7' >= c)
      || U'\u02DE' == c || U'\u02DF' == c || U'\u02ED' == c
      || (U'\u02EF' <= c && U'\u02FF' >= c)
      || U'\u05F3' == c || U'\uA720' == c || U'\uA721' == c || U'\uA789' == c || U'\uA78A' == c || U'\uAB5B' == c
      || U'\u055B' == c || U'\u055C' == c || U'\u055E' == c
    )
    && !Ideographic(c)
    && !Katakana(c)
    && !Linebreak_ComplexContext(c)
    && !Hiragana(c)
    && !Extend(c)
    && !Hebrew_Letter(c);
}

bool WordBreak::Single_Quote(char32_t c) {
  return U'\u0027' == c;
}

bool WordBreak::Double_Quote(char32_t c) {
  return U'\u0022' == c;
}

bool WordBreak::MidNumLet(char32_t c) {
  return U'\u002E' == c || U'\u2018' == c || U'\u2019' == c || U'\u2024' == c || U'\uFE52' == c || U'\uFF07' == c
    || U'\uFF0E' == c;
}

bool WordBreak::MidLetter(char32_t c) {
  return U'\u00B7' == c || U'\u0387' == c || U'\u05F4' == c || U'\u2027' == c || U'\u003A' == c || U'\uFE13' == c
    || U'\uFE55' == c || U'\uFF1A' == c;
}

bool WordBreak::MidNum(char32_t c) {
  return (
      Linebreak_InfixNumeric(c)
      || U'\u066C' == c || U'\uFE50' == c || U'\uFE54' == c || U'\uFF0C' == c || U'\uFF1B' == c
    )
    && !(U'\u003A' == c || U'\uFE13' == c || U'\u002E' == c);
}

bool WordBreak::Numeric(char32_t c) {
  return Linebreak_Numeric(c) && U'\u066C' != c;
}

bool WordBreak::ExtendNumLet(char32_t c) {
  return
    (unicode::category(c) & unicode::Pc)
    || U'\u202F' == c;
}

bool WordBreak::WSegSpace(char32_t c) {
  return
    (unicode::category(c) & unicode::Zs)  // General_Category = Space_Separator
    && !Linebreak_Glue(c);
}

bool WordBreak::AHLetter(char32_t c) {
  return ALetter(c) || Hebrew_Letter(c);
}

bool WordBreak::MidNumLetQ(char32_t c) {
  return MidNumLet(c) || Single_Quote(c);
}

bool WordBreak::Grapheme_Extend(char32_t c) {
  return
    (unicode::category(c) & (unicode::Me | unicode::Mn))
    || _gen_is_other_grapheme_extend(c);
}

bool WordBreak::Emoji_Modifier(char32_t c) {
  return U'\U0001F3FB' <= c && U'\U0001F3FF' >= c;
}

bool WordBreak::Alphabetic(char32_t c) {
  return _gen_is_alphabetic(c);
}

bool WordBreak::Ideographic(char32_t c) {
  return _gen_is_ideographic(c);
}

bool WordBreak::Linebreak_ComplexContext(char32_t c) {
  // http://www.unicode.org/reports/tr14/#SA
  return (unicode::category(c) & (unicode::Cf | unicode::Lo | unicode::Lm | unicode::Mn | unicode::Mc))
    && ((U'\u0E00' <= c && U'\u0E7F' >= c) || (U'\u0E80' <= c && U'\u0EFF' >= c) || (U'\u1000' <= c && U'\u109F' >= c)
      || (U'\u1780' <= c && U'\u17FF' >= c) || (U'\u1950' <= c && U'\u197F' >= c) || (U'\u1980' <= c && U'\u19DF' >= c)
      || (U'\u1A20' <= c && U'\u1AAF' >= c) || (U'\uA9E0' <= c && U'\uA9FF' >= c) || (U'\uAA60' <= c && U'\uAA7F' >= c)
      || (U'\uAA80' <= c && U'\uAADF' >= c) || (U'\U00011700' <= c && U'\U0001173F' >= c));
    // TODO: ...that are not members of another line break class
}

bool WordBreak::Hiragana(char32_t c) {
  return _gen_is_script_hiragana(c);
}

bool WordBreak::Linebreak_Glue(char32_t c) {
  // http://www.unicode.org/reports/tr14/#GL
  return U'\u00A0' == c || U'\u202F' == c || U'\u180E' == c
    || U'\u034F' == c
    || U'\u2007' == c
    || U'\u2011' == c
    || U'\u0F08' == c || U'\u0F0C' == c || U'\u0F12' == c
    || (U'\u035C' <= c && U'\u0362' >= c);

  // TODO: http://www.unicode.org/reports/tr14/#WJ ?
  // || U'\u2060' == c || U'\uFEFF' == c;
}

bool WordBreak::Extended_Pictographic(char32_t c) {
  return _gen_is_extended_pictographic(c);
}

bool WordBreak::Linebreak_InfixNumeric(char32_t c) {
  // http://www.unicode.org/reports/tr14/#IS
  return U'\u002C' == c || U'\u002E' == c || U'\u003A' == c || U'\u003B' == c || U'\u037E' == c || U'\u0589' == c
    || U'\u060C' == c || U'\u060D' == c || U'\u07F8' == c || U'\u2044' == c || U'\uFE10' == c || U'\uFE13' == c
    || U'\uFE14' == c;
}

bool WordBreak::Linebreak_Numeric(char32_t c) {
  // http://www.unicode.org/reports/tr14/#NU
  return (unicode::category(c) & unicode::Nd)
    && !_gen_is_east_asian_width_full(c)
    && U'\u066B' != c && U'\u066C' != c;
}

// Methods below generated with tools/unicode_word_rules.py

bool WordBreak::_gen_is_extended_pictographic(char32_t c) {
  return (U'\U0001FA6E' <= c && U'\U0001FFFD' >= c) || (U'\U0001F442' <= c && U'\U0001F4F7' >= c) || (U'\U0001F266' <= c && U'\U0001F2FF' >= c) || (U'\U0001F030' <= c && U'\U0001F093' >= c) || (U'\U0001FA00' <= c && U'\U0001FA5F' >= c) || (U'\u261A' <= c && U'\u266F' >= c) || (U'\U0001F5A5' <= c && U'\U0001F5FA' >= c) || (U'\U0001F8AE' <= c && U'\U0001F8FF' >= c) || (U'\U0001F337' <= c && U'\U0001F37C' >= c) || (U'\U0001F680' <= c && U'\U0001F6C5' >= c) || (U'\U0001F400' <= c && U'\U0001F43E' >= c) || (U'\U0001F500' <= c && U'\U0001F53D' >= c) || (U'\U0001F1AD' <= c && U'\U0001F1E5' >= c) || (U'\U0001F000' <= c && U'\U0001F02B' >= c) || (U'\U0001F57B' <= c && U'\U0001F5A3' >= c) || (U'\U0001F7D9' <= c && U'\U0001F7FF' >= c) || (U'\U0001F3A0' <= c && U'\U0001F3C4' >= c) || (U'\U0001F300' <= c && U'\U0001F320' >= c) || (U'\U0001F9E7' <= c && U'\U0001F9FF' >= c) || (U'\u26E8' <= c && U'\u26FF' >= c) || (U'\U0001F550' <= c && U'\U0001F567' >= c) || (U'\U0001F9D0' <= c && U'\U0001F9E6' >= c) || (U'\U0001F0E0' <= c && U'\U0001F0F5' >= c) || (U'\U0001F380' <= c && U'\U0001F393' >= c) || (U'\u26CF' <= c && U'\u26E1' >= c) || (U'\U0001F568' <= c && U'\U0001F579' >= c) || (U'\U0001F3E0' <= c && U'\U0001F3F0' >= c) || (U'\u26A2' <= c && U'\u26B1' >= c) || (U'\U0001F601' <= c && U'\U0001F610' >= c) || (U'\U0001F0A0' <= c && U'\U0001F0AE' >= c) || (U'\U0001F0C1' <= c && U'\U0001F0CF' >= c) || (U'\U0001F0D1' <= c && U'\U0001F0DF' >= c) || (U'\U0001F950' <= c && U'\U0001F95E' >= c) || (U'\U0001F0B1' <= c && U'\U0001F0BE' >= c) || (U'\U0001F252' <= c && U'\U0001F25F' >= c) || (U'\U0001FA60' <= c && U'\U0001FA6D' >= c) || (U'\U0001F203' <= c && U'\U0001F20F' >= c) || (U'\U0001F6E0' <= c && U'\U0001F6EC' >= c) || (U'\U0001F95F' <= c && U'\U0001F96B' >= c) || (U'\U0001F985' <= c && U'\U0001F991' >= c) || (U'\U0001F9A3' <= c && U'\U0001F9AF' >= c) || (U'\U0001F9C3' <= c && U'\U0001F9CF' >= c) || (U'\u2607' <= c && U'\u2612' >= c) || (U'\u2672' <= c && U'\u267D' >= c) || (U'\U0001F094' <= c && U'\U0001F09F' >= c) || (U'\U0001F321' <= c && U'\U0001F32C' >= c) || (U'\U0001F394' <= c && U'\U0001F39F' >= c) || (U'\U0001F3D4' <= c && U'\U0001F3DF' >= c) || (U'\U0001F635' <= c && U'\U0001F640' >= c) || (U'\U0001F774' <= c && U'\U0001F77F' >= c) || (U'\u23E9' <= c && U'\u23F3' >= c) || (U'\u2692' <= c && U'\u269C' >= c) || (U'\U0001F645' <= c && U'\U0001F64F' >= c) || (U'\U0001F6D5' <= c && U'\U0001F6DF' >= c) || (U'\U0001F998' <= c && U'\U0001F9A2' >= c) || (U'\u26B3' <= c && U'\u26BC' >= c) || (U'\u26C4' <= c && U'\u26CD' >= c) || (U'\U0001F0F6' <= c && U'\U0001F0FF' >= c) || (U'\U0001F191' <= c && U'\U0001F19A' >= c) || (U'\U0001F6C6' <= c && U'\U0001F6CF' >= c) || (U'\U0001F9B0' <= c && U'\U0001F9B9' >= c) || (U'\U0001F232' <= c && U'\U0001F23A' >= c) || (U'\U0001F910' <= c && U'\U0001F918' >= c) || (U'\U0001F848' <= c && U'\U0001F84F' >= c) || (U'\U0001F888' <= c && U'\U0001F88F' >= c) || (U'\U0001F920' <= c && U'\U0001F927' >= c) || (U'\U0001F928' <= c && U'\U0001F92F' >= c) || (U'\U0001F933' <= c && U'\U0001F93A' >= c) || (U'\u270C' <= c && U'\u2712' >= c) || (U'\U0001F249' <= c && U'\U0001F24F' >= c) || (U'\U0001F3F1' <= c && U'\U0001F3F7' >= c) || (U'\u2194' <= c && U'\u2199' >= c) || (U'\u2600' <= c && U'\u2605' >= c) || (U'\u2680' <= c && U'\u2685' >= c) || (U'\U0001F260' <= c && U'\U0001F265' >= c) || (U'\U0001F330' <= c && U'\U0001F335' >= c) || (U'\U0001F620' <= c && U'\U0001F625' >= c) || (U'\U0001F6FA' <= c && U'\U0001F6FF' >= c) || (U'\U0001F85A' <= c && U'\U0001F85F' >= c) || (U'\U0001F919' <= c && U'\U0001F91E' >= c) || (U'\U0001F940' <= c && U'\U0001F945' >= c) || (U'\U0001F992' <= c && U'\U0001F997' >= c) || (U'\U0001F9BA' <= c && U'\U0001F9BF' >= c) || (U'\u2763' <= c && U'\u2767' >= c) || (U'\U0001F3C6' <= c && U'\U0001F3CA' >= c) || (U'\U0001F3CF' <= c && U'\U0001F3D3' >= c) || (U'\U0001F546' <= c && U'\U0001F54A' >= c) || (U'\U0001F54B' <= c && U'\U0001F54F' >= c) || (U'\U0001F5FB' <= c && U'\U0001F5FF' >= c) || (U'\U0001F947' <= c && U'\U0001F94B' >= c) || (U'\U0001F96C' <= c && U'\U0001F970' >= c) || (U'\U0001F980' <= c && U'\U0001F984' >= c) || (U'\u25FB' <= c && U'\u25FE' >= c) || (U'\u26C0' <= c && U'\u26C3' >= c) || (U'\u26E4' <= c && U'\u26E7' >= c) || (U'\u2701' <= c && U'\u2704' >= c) || (U'\U0001F02C' <= c && U'\U0001F02F' >= c) || (U'\U0001F16C' <= c && U'\U0001F16F' >= c) || (U'\U0001F23C' <= c && U'\U0001F23F' >= c) || (U'\U0001F3CB' <= c && U'\U0001F3CE' >= c) || (U'\U0001F4F9' <= c && U'\U0001F4FC' >= c) || (U'\U0001F628' <= c && U'\U0001F62B' >= c) || (U'\U0001F630' <= c && U'\U0001F633' >= c) || (U'\U0001F6F0' <= c && U'\U0001F6F3' >= c) || (U'\U0001F7D5' <= c && U'\U0001F7D8' >= c) || (U'\U0001F80C' <= c && U'\U0001F80F' >= c) || (U'\U0001F90C' <= c && U'\U0001F90F' >= c) || (U'\U0001F973' <= c && U'\U0001F976' >= c) || (U'\U0001F97C' <= c && U'\U0001F97F' >= c) || U'\u00A9' == c || U'\u00AE' == c || U'\u203C' == c || U'\u2049' == c || U'\u2122' == c || U'\u2139' == c || U'\u2328' == c || U'\u2388' == c || U'\u23CF' == c || (U'\u23F8' <= c && U'\u23FA' >= c) || U'\u24C2' == c || U'\u25B6' == c || U'\u25C0' == c || U'\u2618' == c || U'\u2619' == c || U'\u269D' == c || U'\u26B2' == c || (U'\u26BD' <= c && U'\u26BF' >= c) || U'\u26CE' == c || U'\u26E2' == c || U'\u26E3' == c || U'\u2700' == c || U'\u2705' == c || U'\u2714' == c || U'\u2716' == c || U'\u271D' == c || U'\u2721' == c || U'\u2728' == c || U'\u2744' == c || U'\u2747' == c || U'\u274C' == c || U'\u274E' == c || (U'\u2753' <= c && U'\u2755' >= c) || U'\u2757' == c || (U'\u2795' <= c && U'\u2797' >= c) || U'\u27A1' == c || U'\u27B0' == c || U'\u27BF' == c || (U'\u2B05' <= c && U'\u2B07' >= c) || U'\u2B50' == c || U'\u2B55' == c || U'\u3030' == c || U'\u303D' == c || U'\u3297' == c || U'\u3299' == c || U'\U0001F0BF' == c || U'\U0001F0C0' == c || U'\U0001F0D0' == c || (U'\U0001F10D' <= c && U'\U0001F10F' >= c) || U'\U0001F12F' == c || U'\U0001F17E' == c || U'\U0001F17F' == c || U'\U0001F18E' == c || U'\U0001F21A' == c || U'\U0001F22F' == c || (U'\U0001F32D' <= c && U'\U0001F32F' >= c) || U'\U0001F336' == c || U'\U0001F37D' == c || U'\U0001F3C5' == c || (U'\U0001F3F8' <= c && U'\U0001F3FA' >= c) || U'\U0001F43F' == c || U'\U0001F440' == c || U'\U0001F441' == c || U'\U0001F4F8' == c || U'\U0001F4FF' == c || U'\U0001F57A' == c || U'\U0001F5A4' == c || U'\U0001F600' == c || U'\U0001F611' == c || (U'\U0001F612' <= c && U'\U0001F614' >= c) || U'\U0001F615' == c || U'\U0001F616' == c || U'\U0001F617' == c || U'\U0001F618' == c || U'\U0001F619' == c || U'\U0001F61A' == c || U'\U0001F61B' == c || (U'\U0001F61C' <= c && U'\U0001F61E' >= c) || U'\U0001F61F' == c || U'\U0001F62C' == c || U'\U0001F62D' == c || U'\U0001F634' == c || U'\U0001F6D0' == c || (U'\U0001F6ED' <= c && U'\U0001F6EF' >= c) || (U'\U0001F6F4' <= c && U'\U0001F6F6' >= c) || U'\U0001F6F9' == c || U'\U0001F91F' == c || U'\U0001F930' == c || (U'\U0001F93C' <= c && U'\U0001F93E' >= c) || U'\U0001F93F' == c || U'\U0001F94C' == c || (U'\U0001F94D' <= c && U'\U0001F94F' >= c) || (U'\U0001F977' <= c && U'\U0001F979' >= c) || U'\U0001F97A' == c || U'\U0001F97B' == c || U'\U0001F9C0' == c || U'\u21A9' == c || U'\u21AA' == c || U'\u231A' == c || U'\u231B' == c || U'\u25AA' == c || U'\u25AB' == c || U'\u2614' == c || U'\u2615' == c || U'\u2616' == c || U'\u2617' == c || U'\u2670' == c || U'\u2671' == c || U'\u267E' == c || U'\u267F' == c || U'\u2690' == c || U'\u2691' == c || U'\u269E' == c || U'\u269F' == c || U'\u26A0' == c || U'\u26A1' == c || U'\u2708' == c || U'\u2709' == c || U'\u270A' == c || U'\u270B' == c || U'\u2733' == c || U'\u2734' == c || U'\u2934' == c || U'\u2935' == c || U'\u2B1B' == c || U'\u2B1C' == c || U'\U0001F0AF' == c || U'\U0001F0B0' == c || U'\U0001F170' == c || U'\U0001F171' == c || U'\U0001F201' == c || U'\U0001F202' == c || U'\U0001F250' == c || U'\U0001F251' == c || U'\U0001F37E' == c || U'\U0001F37F' == c || U'\U0001F4FD' == c || U'\U0001F4FE' == c || U'\U0001F626' == c || U'\U0001F627' == c || U'\U0001F62E' == c || U'\U0001F62F' == c || U'\U0001F641' == c || U'\U0001F642' == c || U'\U0001F643' == c || U'\U0001F644' == c || U'\U0001F6D1' == c || U'\U0001F6D2' == c || U'\U0001F6D3' == c || U'\U0001F6D4' == c || U'\U0001F6F7' == c || U'\U0001F6F8' == c || U'\U0001F931' == c || U'\U0001F932' == c || U'\U0001F971' == c || U'\U0001F972' == c || U'\U0001F9C1' == c || U'\U0001F9C2' == c;
}

bool WordBreak::_gen_is_other_grapheme_extend(char32_t c) {
  return (U'\U000E0020' <= c && U'\U000E007F' >= c) || (U'\U0001D16E' <= c && U'\U0001D172' >= c) || U'\u09BE' == c || U'\u09D7' == c || U'\u0B3E' == c || U'\u0B57' == c || U'\u0BBE' == c || U'\u0BD7' == c || U'\u0CC2' == c || U'\u0D3E' == c || U'\u0D57' == c || U'\u0DCF' == c || U'\u0DDF' == c || U'\u200C' == c || U'\U0001133E' == c || U'\U00011357' == c || U'\U000114B0' == c || U'\U000114BD' == c || U'\U000115AF' == c || U'\U0001D165' == c || U'\u0CD5' == c || U'\u0CD6' == c || U'\u302E' == c || U'\u302F' == c || U'\uFF9E' == c || U'\uFF9F' == c;
}

bool WordBreak::_gen_is_alphabetic(char32_t c) {
  return (U'\U00020000' <= c && U'\U0002A6D6' >= c) || (U'\u4E00' <= c && U'\u9FEF' >= c) || (U'\uAC00' <= c && U'\uD7A3' >= c) || (U'\U0002CEB0' <= c && U'\U0002EBE0' >= c) || (U'\u3400' <= c && U'\u4DB5' >= c) || (U'\U00017000' <= c && U'\U000187F1' >= c) || (U'\U0002B820' <= c && U'\U0002CEA1' >= c) || (U'\U0002A700' <= c && U'\U0002B734' >= c) || (U'\uA016' <= c && U'\uA48C' >= c) || (U'\U00013000' <= c && U'\U0001342E' >= c) || (U'\U00012000' <= c && U'\U00012399' >= c) || (U'\U00018800' <= c && U'\U00018AF2' >= c) || (U'\u1401' <= c && U'\u166C' >= c) || (U'\U00014400' <= c && U'\U00014646' >= c) || (U'\U00016800' <= c && U'\U00016A38' >= c) || (U'\U0002F800' <= c && U'\U0002FA1D' >= c) || (U'\U0001B170' <= c && U'\U0001B2FB' >= c) || (U'\uF900' <= c && U'\uFA6D' >= c) || (U'\uFBD3' <= c && U'\uFD3D' >= c) || (U'\U0001D552' <= c && U'\U0001D6A5' >= c) || (U'\u1100' <= c && U'\u1248' >= c) || (U'\U00010600' <= c && U'\U00010736' >= c) || (U'\U0001B000' <= c && U'\U0001B11E' >= c) || (U'\u1E00' <= c && U'\u1F15' >= c) || (U'\uA500' <= c && U'\uA60B' >= c) || (U'\U0002B740' <= c && U'\U0002B81D' >= c) || (U'\u01C4' <= c && U'\u0293' >= c) || (U'\U0001E800' <= c && U'\U0001E8C4' >= c) || (U'\U00012480' <= c && U'\U00012543' >= c) || (U'\u00F8' <= c && U'\u01BA' >= c) || (U'\u048A' <= c && U'\u052F' >= c) || (U'\u03F7' <= c && U'\u0481' >= c) || (U'\uFE76' <= c && U'\uFEFC' >= c) || (U'\U00010080' <= c && U'\U000100FA' >= c) || (U'\U00012400' <= c && U'\U0001246E' >= c) || (U'\uFB46' <= c && U'\uFBB1' >= c) || (U'\U0001BC00' <= c && U'\U0001BC6A' >= c) || (U'\uFA70' <= c && U'\uFAD9' >= c) || (U'\u2C7E' <= c && U'\u2CE4' >= c) || (U'\u0671' <= c && U'\u06D3' >= c) || (U'\u3131' <= c && U'\u318E' >= c) || (U'\u30A1' <= c && U'\u30FA' >= c) || (U'\u074D' <= c && U'\u07A5' >= c) || (U'\u13A0' <= c && U'\u13F5' >= c) || (U'\u3041' <= c && U'\u3096' >= c) || (U'\U0001D400' <= c && U'\U0001D454' >= c) || (U'\u03A3' <= c && U'\u03F5' >= c) || (U'\uAB70' <= c && U'\uABBF' >= c) || (U'\U00010400' <= c && U'\U0001044F' >= c) || (U'\uA722' <= c && U'\uA76F' >= c) || (U'\U00010450' <= c && U'\U0001049D' >= c) || (U'\u16A0' <= c && U'\u16EA' >= c) || (U'\U00010C00' <= c && U'\U00010C48' >= c) || (U'\U0001D456' <= c && U'\U0001D49C' >= c) || (U'\u18B0' <= c && U'\u18F5' >= c) || (U'\uA6A0' <= c && U'\uA6E5' >= c) || (U'\U00016F00' <= c && U'\U00016F44' >= c) || (U'\U0001E900' <= c && U'\U0001E943' >= c) || (U'\u1318' <= c && U'\u135A' >= c) || (U'\U0001D4C5' <= c && U'\U0001D505' >= c) || (U'\uFD50' <= c && U'\uFD8F' >= c) || (U'\U000118A0' <= c && U'\U000118DF' >= c) || (U'\U00016E40' <= c && U'\U00016E7F' >= c) || (U'\u1D2C' <= c && U'\u1D6A' >= c) || (U'\u12D8' <= c && U'\u1310' >= c) || (U'\U00011AC0' <= c && U'\U00011AF8' >= c) || (U'\u2D30' <= c && U'\u2D67' >= c) || (U'\U00010980' <= c && U'\U000109B7' >= c) || (U'\u0904' <= c && U'\u0939' >= c) || (U'\uFD92' <= c && U'\uFDC7' >= c) || (U'\U00010B00' <= c && U'\U00010B35' >= c) || (U'\u1844' <= c && U'\u1878' >= c) || (U'\u1A20' <= c && U'\u1A54' >= c) || (U'\u1F80' <= c && U'\u1FB4' >= c) || (U'\U00010140' <= c && U'\U00010174' >= c) || (U'\U00011003' <= c && U'\U00011037' >= c) || (U'\U00011400' <= c && U'\U00011434' >= c) || (U'\u1780' <= c && U'\u17B3' >= c) || (U'\u24B6' <= c && U'\u24E9' >= c) || (U'\uA840' <= c && U'\uA873' >= c) || (U'\U00010530' <= c && U'\U00010563' >= c) || (U'\U00010C80' <= c && U'\U00010CB2' >= c) || (U'\U00010CC0' <= c && U'\U00010CF2' >= c) || (U'\uA882' <= c && U'\uA8B3' >= c) || (U'\uAA7E' <= c && U'\uAAAF' >= c) || (U'\uD7CB' <= c && U'\uD7FB' >= c) || (U'\U000102A0' <= c && U'\U000102D0' >= c) || (U'\u0E01' <= c && U'\u0E30' >= c) || (U'\U00011183' <= c && U'\U000111B2' >= c) || (U'\U00011480' <= c && U'\U000114AF' >= c) || (U'\U00011600' <= c && U'\U0001162F' >= c) || (U'\U00016B00' <= c && U'\U00016B2F' >= c) || (U'\u1B05' <= c && U'\u1B33' >= c) || (U'\u2C00' <= c && U'\u2C2E' >= c) || (U'\u2C30' <= c && U'\u2C5E' >= c) || (U'\uA984' <= c && U'\uA9B2' >= c) || (U'\U000112B0' <= c && U'\U000112DE' >= c) || (U'\U00011580' <= c && U'\U000115AE' >= c) || (U'\uA640' <= c && U'\uA66D' >= c) || (U'\U00016F51' <= c && U'\U00016F7E' >= c) || (U'\uFF71' <= c && U'\uFF9D' >= c) || (U'\U00011083' <= c && U'\U000110AF' >= c) || (U'\u1980' <= c && U'\u19AB' >= c) || (U'\u1BBA' <= c && U'\u1BE5' >= c) || (U'\u1D00' <= c && U'\u1D2B' >= c) || (U'\U0001080A' <= c && U'\U00010835' >= c) || (U'\U00011800' <= c && U'\U0001182B' >= c) || (U'\u1000' <= c && U'\u102A' >= c) || (U'\u10D0' <= c && U'\u10FA' >= c) || (U'\u1C90' <= c && U'\u1CBA' >= c) || (U'\u3105' <= c && U'\u312F' >= c) || (U'\uAB30' <= c && U'\uAB5A' >= c) || (U'\U00011680' <= c && U'\U000116AA' >= c) || (U'\uA790' <= c && U'\uA7B9' >= c) || (U'\u0560' <= c && U'\u0588' >= c) || (U'\u0D12' <= c && U'\u0D3A' >= c) || (U'\u1260' <= c && U'\u1288' >= c) || (U'\uAA00' <= c && U'\uAA28' >= c) || (U'\uA4D0' <= c && U'\uA4F7' >= c) || (U'\U00010500' <= c && U'\U00010527' >= c) || (U'\U00011A0B' <= c && U'\U00011A32' >= c) || (U'\U00011A5C' <= c && U'\U00011A83' >= c) || (U'\u0531' <= c && U'\u0556' >= c) || (U'\u10A0' <= c && U'\u10C5' >= c) || (U'\u1F20' <= c && U'\u1F45' >= c) || (U'\u2D00' <= c && U'\u2D25' >= c) || (U'\U00010350' <= c && U'\U00010375' >= c) || (U'\U00011D0B' <= c && U'\U00011D30' >= c) || (U'\u1D9B' <= c && U'\u1DBF' >= c) || (U'\U00011C0A' <= c && U'\U00011C2E' >= c) || (U'\u0F49' <= c && U'\u0F6C' >= c) || (U'\u0F99' <= c && U'\u0FBC' >= c) || (U'\u1C00' <= c && U'\u1C23' >= c) || (U'\U000103A0' <= c && U'\U000103C3' >= c) || (U'\U000104B0' <= c && U'\U000104D3' >= c) || (U'\U000104D8' <= c && U'\U000104FB' >= c) || (U'\U00010D00' <= c && U'\U00010D23' >= c) || (U'\U00011103' <= c && U'\U00011126' >= c) || (U'\u1820' <= c && U'\u1842' >= c) || (U'\u2160' <= c && U'\u2182' >= c) || (U'\uABC0' <= c && U'\uABE2' >= c) || (U'\U00011150' <= c && U'\U00011172' >= c) || (U'\u1887' <= c && U'\u18A8' >= c) || (U'\u1D79' <= c && U'\u1D9A' >= c) || (U'\u07CA' <= c && U'\u07EA' >= c) || (U'\u1290' <= c && U'\u12B0' >= c) || (U'\u0620' <= c && U'\u063F' >= c) || (U'\u2DE0' <= c && U'\u2DFF' >= c) || (U'\U00010300' <= c && U'\U0001031F' >= c) || (U'\U00011D6A' <= c && U'\U00011D89' >= c) || (U'\u00D8' <= c && U'\u00F6' >= c) || (U'\u1900' <= c && U'\u191E' >= c) || (U'\u1F5F' <= c && U'\u1F7D' >= c) || (U'\uFFA0' <= c && U'\uFFBE' >= c) || (U'\U00010880' <= c && U'\U0001089E' >= c) || (U'\U00016A40' <= c && U'\U00016A5E' >= c) || (U'\U0001D6DC' <= c && U'\U0001D6FA' >= c) || (U'\U0001D716' <= c && U'\U0001D734' >= c) || (U'\U0001D750' <= c && U'\U0001D76E' >= c) || (U'\U0001D78A' <= c && U'\U0001D7A8' >= c) || (U'\u0712' <= c && U'\u072F' >= c) || (U'\u1950' <= c && U'\u196D' >= c) || (U'\u1B83' <= c && U'\u1BA0' >= c) || (U'\u1C5A' <= c && U'\u1C77' >= c) || (U'\U00010380' <= c && U'\U0001039D' >= c) || (U'\U00011C72' <= c && U'\U00011C8F' >= c) || (U'\U00016AD0' <= c && U'\U00016AED' >= c) || (U'\uA960' <= c && U'\uA97C' >= c) || (U'\U00010280' <= c && U'\U0001029C' >= c) || (U'\U00010A19' <= c && U'\U00010A35' >= c) || (U'\U00010A60' <= c && U'\U00010A7C' >= c) || (U'\U00010A80' <= c && U'\U00010A9C' >= c) || (U'\U00010F00' <= c && U'\U00010F1C' >= c) || (U'\u2C60' <= c && U'\u2C7B' >= c) || (U'\uA680' <= c && U'\uA69B' >= c) || (U'\uA90A' <= c && U'\uA925' >= c) || (U'\U00010AC9' <= c && U'\U00010AE4' >= c) || (U'\U0001D51E' <= c && U'\U0001D539' >= c) || (U'\u0295' <= c && U'\u02AF' >= c) || (U'\u05D0' <= c && U'\u05EA' >= c) || (U'\u31A0' <= c && U'\u31BA' >= c) || (U'\U00011700' <= c && U'\U0001171A' >= c) || (U'\U0001EE05' <= c && U'\U0001EE1F' >= c) || (U'\u0041' <= c && U'\u005A' >= c) || (U'\u0061' <= c && U'\u007A' >= c) || (U'\u1681' <= c && U'\u169A' >= c) || (U'\u19B0' <= c && U'\u19C9' >= c) || (U'\uFF21' <= c && U'\uFF3A' >= c) || (U'\uFF41' <= c && U'\uFF5A' >= c) || (U'\U0001000D' <= c && U'\U00010026' >= c) || (U'\U00010920' <= c && U'\U00010939' >= c) || (U'\U0001F130' <= c && U'\U0001F149' >= c) || (U'\U0001F150' <= c && U'\U0001F169' >= c) || (U'\U0001F170' <= c && U'\U0001F189' >= c) || (U'\u0840' <= c && U'\u0858' >= c) || (U'\U000110D0' <= c && U'\U000110E8' >= c) || (U'\U00011213' <= c && U'\U0001122B' >= c) || (U'\U0001D6A8' <= c && U'\U0001D6C0' >= c) || (U'\U0001D6C2' <= c && U'\U0001D6DA' >= c) || (U'\U0001D6FC' <= c && U'\U0001D714' >= c) || (U'\U0001D736' <= c && U'\U0001D74E' >= c) || (U'\U0001D770' <= c && U'\U0001D788' >= c) || (U'\U0001D7AA' <= c && U'\U0001D7C2' >= c) || (U'\u0D9A' <= c && U'\u0DB1' >= c) || (U'\u00C0' <= c && U'\u00D6' >= c) || (U'\u0C12' <= c && U'\u0C28' >= c) || (U'\u0C92' <= c && U'\u0CA8' >= c) || (U'\u1A00' <= c && U'\u1A16' >= c) || (U'\u2D80' <= c && U'\u2D96' >= c) || (U'\uA771' <= c && U'\uA787' >= c) || (U'\uA80C' <= c && U'\uA822' >= c) || (U'\uA930' <= c && U'\uA946' >= c) || (U'\uD7B0' <= c && U'\uD7C6' >= c) || (U'\U0001083F' <= c && U'\U00010855' >= c) || (U'\U00010860' <= c && U'\U00010876' >= c) || (U'\u0800' <= c && U'\u0815' >= c) || (U'\u0993' <= c && U'\u09A8' >= c) || (U'\u0A13' <= c && U'\u0A28' >= c) || (U'\u0A93' <= c && U'\u0AA8' >= c) || (U'\u0B13' <= c && U'\u0B28' >= c) || (U'\U00010740' <= c && U'\U00010755' >= c) || (U'\U00010900' <= c && U'\U00010915' >= c) || (U'\U00010B40' <= c && U'\U00010B55' >= c) || (U'\U00010F30' <= c && U'\U00010F45' >= c) || (U'\U00011313' <= c && U'\U00011328' >= c) || (U'\U00011C92' <= c && U'\U00011CA7' >= c) || (U'\u08A0' <= c && U'\u08B4' >= c) || (U'\uA000' <= c && U'\uA014' >= c) || (U'\U00016B63' <= c && U'\U00016B77' >= c) || (U'\u038E' <= c && U'\u03A1' >= c) || (U'\U0001032D' <= c && U'\U00010340' >= c) || (U'\u08F0' <= c && U'\u0902' >= c) || (U'\U00010028' <= c && U'\U0001003A' >= c) || (U'\U000108E0' <= c && U'\U000108F2' >= c) || (U'\U00010B60' <= c && U'\U00010B72' >= c) || (U'\U00011EE0' <= c && U'\U00011EF2' >= c) || (U'\U00016B7D' <= c && U'\U00016B8F' >= c) || (U'\u02B0' <= c && U'\u02C1' >= c) || (U'\u0D85' <= c && U'\u0D96' >= c) || (U'\u1720' <= c && U'\u1731' >= c) || (U'\u1740' <= c && U'\u1751' >= c) || (U'\U00010B80' <= c && U'\U00010B91' >= c) || (U'\U00011200' <= c && U'\U00011211' >= c) || (U'\u166F' <= c && U'\u167F' >= c) || (U'\U0001E008' <= c && U'\U0001E018' >= c) || (U'\U0001EE8B' <= c && U'\U0001EE9B' >= c) || (U'\U0001EEAB' <= c && U'\U0001EEBB' >= c) || (U'\u0730' <= c && U'\u073F' >= c) || (U'\u0C2A' <= c && U'\u0C39' >= c) || (U'\u1380' <= c && U'\u138F' >= c) || (U'\u31F0' <= c && U'\u31FF' >= c) || (U'\uA610' <= c && U'\uA61F' >= c) || (U'\uA8B4' <= c && U'\uA8C3' >= c) || (U'\uAA60' <= c && U'\uAA6F' >= c) || (U'\u0972' <= c && U'\u0980' >= c) || (U'\u12C8' <= c && U'\u12D6' >= c) || (U'\U0001003F' <= c && U'\U0001004D' >= c) || (U'\U0001128F' <= c && U'\U0001129D' >= c) || (U'\u05B0' <= c && U'\u05BD' >= c) || (U'\u0F71' <= c && U'\u0F7E' >= c) || (U'\u1DE7' <= c && U'\u1DF4' >= c) || (U'\U00010050' <= c && U'\U0001005D' >= c) || (U'\U00011038' <= c && U'\U00011045' >= c) || (U'\u064B' <= c && U'\u0657' >= c) || (U'\u1075' <= c && U'\u1081' >= c) || (U'\u1700' <= c && U'\u170C' >= c) || (U'\u1760' <= c && U'\u176C' >= c) || (U'\u1D6B' <= c && U'\u1D77' >= c) || (U'\u1FE0' <= c && U'\u1FEC' >= c) || (U'\u2090' <= c && U'\u209C' >= c) || (U'\uFB2A' <= c && U'\uFB36' >= c) || (U'\U00011A8A' <= c && U'\U00011A96' >= c) || (U'\U00016F93' <= c && U'\U00016F9F' >= c) || (U'\U0001BC70' <= c && U'\U0001BC7C' >= c) || (U'\u02C6' <= c && U'\u02D1' >= c) || (U'\u08D4' <= c && U'\u08DF' >= c) || (U'\u0BAE' <= c && U'\u0BB9' >= c) || (U'\uFDF0' <= c && U'\uFDFB' >= c) || (U'\U00010000' <= c && U'\U0001000B' >= c) || (U'\U0001D4AE' <= c && U'\U0001D4B9' >= c) || (U'\u0610' <= c && U'\u061A' >= c) || (U'\u07A6' <= c && U'\u07B0' >= c) || (U'\u0860' <= c && U'\u086A' >= c) || (U'\u0F8D' <= c && U'\u0F97' >= c) || (U'\uA947' <= c && U'\uA951' >= c) || (U'\uAAE0' <= c && U'\uAAEA' >= c) || (U'\u0641' <= c && U'\u064A' >= c) || (U'\u0958' <= c && U'\u0961' >= c) || (U'\u0CAA' <= c && U'\u0CB3' >= c) || (U'\u210A' <= c && U'\u2113' >= c) || (U'\uA6E6' <= c && U'\uA6EF' >= c) || (U'\uFB1F' <= c && U'\uFB28' >= c) || (U'\uFF66' <= c && U'\uFF6F' >= c) || (U'\U0001129F' <= c && U'\U000112A8' >= c) || (U'\U00011A01' <= c && U'\U00011A0A' >= c) || (U'\U0001BC90' <= c && U'\U0001BC99' >= c) || (U'\U0001EE29' <= c && U'\U0001EE32' >= c) || (U'\U0001EE80' <= c && U'\U0001EE89' >= c) || (U'\u081B' <= c && U'\u0823' >= c) || (U'\u0A85' <= c && U'\u0A8D' >= c) || (U'\u0DB3' <= c && U'\u0DBB' >= c) || (U'\u1C80' <= c && U'\u1C88' >= c) || (U'\u3021' <= c && U'\u3029' >= c) || (U'\uA717' <= c && U'\uA71F' >= c) || (U'\uA9E7' <= c && U'\uA9EF' >= c) || (U'\U000111B6' <= c && U'\U000111BE' >= c) || (U'\U0001182F' <= c && U'\U00011837' >= c) || (U'\U00011C00' <= c && U'\U00011C08' >= c) || (U'\U0001BC80' <= c && U'\U0001BC88' >= c) || (U'\u08B6' <= c && U'\u08BD' >= c) || (U'\u0941' <= c && U'\u0948' >= c) || (U'\u0985' <= c && U'\u098C' >= c) || (U'\u0B05' <= c && U'\u0B0C' >= c) || (U'\u0C05' <= c && U'\u0C0C' >= c) || (U'\u0C85' <= c && U'\u0C8C' >= c) || (U'\u0D05' <= c && U'\u0D0C' >= c) || (U'\u0DD8' <= c && U'\u0DDF' >= c) || (U'\u0F40' <= c && U'\u0F47' >= c) || (U'\u16F1' <= c && U'\u16F8' >= c) || (U'\u17BE' <= c && U'\u17C5' >= c) || (U'\u1A65' <= c && U'\u1A6C' >= c) || (U'\u1C24' <= c && U'\u1C2B' >= c) || (U'\u1C2C' <= c && U'\u1C33' >= c) || (U'\u1F50' <= c && U'\u1F57' >= c) || (U'\uA674' <= c && U'\uA67B' >= c) || (U'\uAA44' <= c && U'\uAA4B' >= c) || (U'\U00010342' <= c && U'\U00010349' >= c) || (U'\U000103C8' <= c && U'\U000103CF' >= c) || (U'\U00010760' <= c && U'\U00010767' >= c) || (U'\U00010AC0' <= c && U'\U00010AC7' >= c) || (U'\U00011305' <= c && U'\U0001130C' >= c) || (U'\U00011438' <= c && U'\U0001143F' >= c) || (U'\U00011633' <= c && U'\U0001163A' >= c) || (U'\U0001D50D' <= c && U'\U0001D514' >= c) || (U'\U0001D7C4' <= c && U'\U0001D7CB' >= c) || (U'\u0659' <= c && U'\u065F' >= c) || (U'\u06D6' <= c && U'\u06DC' >= c) || (U'\u08E3' <= c && U'\u08E9' >= c) || (U'\u09AA' <= c && U'\u09B0' >= c) || (U'\u0A2A' <= c && U'\u0A30' >= c) || (U'\u0AAA' <= c && U'\u0AB0' >= c) || (U'\u0B2A' <= c && U'\u0B30' >= c) || (U'\u0DC0' <= c && U'\u0DC6' >= c) || (U'\u0E34' <= c && U'\u0E3A' >= c) || (U'\u0E99' <= c && U'\u0E9F' >= c) || (U'\u1250' <= c && U'\u1256' >= c) || (U'\u12B8' <= c && U'\u12BE' >= c) || (U'\u17B7' <= c && U'\u17BD' >= c) || (U'\u1A58' <= c && U'\u1A5E' >= c) || (U'\u1B45' <= c && U'\u1B4B' >= c) || (U'\u1FB6' <= c && U'\u1FBC' >= c) || (U'\u1FC6' <= c && U'\u1FCC' >= c) || (U'\u1FF6' <= c && U'\u1FFC' >= c) || (U'\u2DA0' <= c && U'\u2DA6' >= c) || (U'\u2DA8' <= c && U'\u2DAE' >= c) || (U'\u2DB0' <= c && U'\u2DB6' >= c) || (U'\u2DB8' <= c && U'\u2DBE' >= c) || (U'\u2DC0' <= c && U'\u2DC6' >= c) || (U'\u2DC8' <= c && U'\u2DCE' >= c) || (U'\u2DD0' <= c && U'\u2DD6' >= c) || (U'\u2DD8' <= c && U'\u2DDE' >= c) || (U'\uA7FB' <= c && U'\uA801' >= c) || (U'\uAB20' <= c && U'\uAB26' >= c) || (U'\uAB28' <= c && U'\uAB2E' >= c) || (U'\uFB00' <= c && U'\uFB06' >= c) || (U'\U00011280' <= c && U'\U00011286' >= c) || (U'\U0001132A' <= c && U'\U00011330' >= c) || (U'\U00011C30' <= c && U'\U00011C36' >= c) || (U'\U00011CAA' <= c && U'\U00011CB0' >= c) || (U'\U00011D00' <= c && U'\U00011D06' >= c) || (U'\U00016B30' <= c && U'\U00016B36' >= c) || (U'\U0001D4BD' <= c && U'\U0001D4C3' >= c) || (U'\U0001D516' <= c && U'\U0001D51C' >= c) || (U'\U0001D54A' <= c && U'\U0001D550' >= c) || (U'\U0001E000' <= c && U'\U0001E006' >= c) || (U'\U0001E01B' <= c && U'\U0001E021' >= c) || (U'\U0001EE6C' <= c && U'\U0001EE72' >= c) || (U'\u0A05' <= c && U'\u0A0A' >= c) || (U'\u0B85' <= c && U'\u0B8A' >= c) || (U'\u0D7A' <= c && U'\u0D7F' >= c) || (U'\u0E40' <= c && U'\u0E45' >= c) || (U'\u0EB4' <= c && U'\u0EB9' >= c) || (U'\u1050' <= c && U'\u1055' >= c) || (U'\u13F8' <= c && U'\u13FD' >= c) || (U'\u1933' <= c && U'\u1938' >= c) || (U'\u1A6D' <= c && U'\u1A72' >= c) || (U'\u1C78' <= c && U'\u1C7D' >= c) || (U'\u1F18' <= c && U'\u1F1D' >= c) || (U'\u1F48' <= c && U'\u1F4D' >= c) || (U'\u1FD6' <= c && U'\u1FDB' >= c) || (U'\u212F' <= c && U'\u2134' >= c) || (U'\uA4F8' <= c && U'\uA4FD' >= c) || (U'\uA8F2' <= c && U'\uA8F7' >= c) || (U'\uAA29' <= c && U'\uAA2E' >= c) || (U'\uAA71' <= c && U'\uAA76' >= c) || (U'\uAB01' <= c && U'\uAB06' >= c) || (U'\uAB09' <= c && U'\uAB0E' >= c) || (U'\uAB11' <= c && U'\uAB16' >= c) || (U'\uAB60' <= c && U'\uAB65' >= c) || (U'\uFFC2' <= c && U'\uFFC7' >= c) || (U'\uFFCA' <= c && U'\uFFCF' >= c) || (U'\uFFD2' <= c && U'\uFFD7' >= c) || (U'\U00010800' <= c && U'\U00010805' >= c) || (U'\U0001112D' <= c && U'\U00011132' >= c) || (U'\U000112E3' <= c && U'\U000112E8' >= c) || (U'\U000114B3' <= c && U'\U000114B8' >= c) || (U'\U000116B0' <= c && U'\U000116B5' >= c) || (U'\U00011A51' <= c && U'\U00011A56' >= c) || (U'\U00011C38' <= c && U'\U00011C3D' >= c) || (U'\U00011D31' <= c && U'\U00011D36' >= c) || (U'\U00011D60' <= c && U'\U00011D65' >= c) || (U'\u02E0' <= c && U'\u02E4' >= c) || (U'\u0AB5' <= c && U'\u0AB9' >= c) || (U'\u0AC1' <= c && U'\u0AC5' >= c) || (U'\u0B35' <= c && U'\u0B39' >= c) || (U'\u0CB5' <= c && U'\u0CB9' >= c) || (U'\u0CC0' <= c && U'\u0CC4' >= c) || (U'\u0EC0' <= c && U'\u0EC4' >= c) || (U'\u0F88' <= c && U'\u0F8C' >= c) || (U'\u1032' <= c && U'\u1036' >= c) || (U'\u1880' <= c && U'\u1884' >= c) || (U'\u1970' <= c && U'\u1974' >= c) || (U'\u1B36' <= c && U'\u1B3A' >= c) || (U'\u1B3D' <= c && U'\u1B41' >= c) || (U'\u2119' <= c && U'\u211D' >= c) || (U'\u2145' <= c && U'\u2149' >= c) || (U'\u3031' <= c && U'\u3035' >= c) || (U'\uA926' <= c && U'\uA92A' >= c) || (U'\uA9E0' <= c && U'\uA9E4' >= c) || (U'\uA9FA' <= c && U'\uA9FE' >= c) || (U'\uAAB9' <= c && U'\uAABD' >= c) || (U'\uFB13' <= c && U'\uFB17' >= c) || (U'\uFB38' <= c && U'\uFB3C' >= c) || (U'\uFE70' <= c && U'\uFE74' >= c) || (U'\U00010376' <= c && U'\U0001037A' >= c) || (U'\U000103D1' <= c && U'\U000103D5' >= c) || (U'\U00011127' <= c && U'\U0001112B' >= c) || (U'\U00011335' <= c && U'\U00011339' >= c) || (U'\U0001135D' <= c && U'\U00011361' >= c) || (U'\U00011D8A' <= c && U'\U00011D8E' >= c) || (U'\U0001D540' <= c && U'\U0001D544' >= c) || (U'\U0001E026' <= c && U'\U0001E02A' >= c) || (U'\U0001EEA5' <= c && U'\U0001EEA9' >= c) || (U'\u01BC' <= c && U'\u01BF' >= c) || (U'\u01C0' <= c && U'\u01C3' >= c) || (U'\u0370' <= c && U'\u0373' >= c) || (U'\u05EF' <= c && U'\u05F2' >= c) || (U'\u06E1' <= c && U'\u06E4' >= c) || (U'\u0829' <= c && U'\u082C' >= c) || (U'\u0949' <= c && U'\u094C' >= c) || (U'\u09B6' <= c && U'\u09B9' >= c) || (U'\u09C1' <= c && U'\u09C4' >= c) || (U'\u0A59' <= c && U'\u0A5C' >= c) || (U'\u0B41' <= c && U'\u0B44' >= c) || (U'\u0B92' <= c && U'\u0B95' >= c) || (U'\u0C41' <= c && U'\u0C44' >= c) || (U'\u0D41' <= c && U'\u0D44' >= c) || (U'\u0E94' <= c && U'\u0E97' >= c) || (U'\u0EAD' <= c && U'\u0EB0' >= c) || (U'\u0EDC' <= c && U'\u0EDF' >= c) || (U'\u102D' <= c && U'\u1030' >= c) || (U'\u105A' <= c && U'\u105D' >= c) || (U'\u1071' <= c && U'\u1074' >= c) || (U'\u124A' <= c && U'\u124D' >= c) || (U'\u125A' <= c && U'\u125D' >= c) || (U'\u128A' <= c && U'\u128D' >= c) || (U'\u12B2' <= c && U'\u12B5' >= c) || (U'\u12C2' <= c && U'\u12C5' >= c) || (U'\u1312' <= c && U'\u1315' >= c) || (U'\u170E' <= c && U'\u1711' >= c) || (U'\u1923' <= c && U'\u1926' >= c) || (U'\u1B00' <= c && U'\u1B03' >= c) || (U'\u1BA2' <= c && U'\u1BA5' >= c) || (U'\u1CE9' <= c && U'\u1CEC' >= c) || (U'\u1CEE' <= c && U'\u1CF1' >= c) || (U'\u1FD0' <= c && U'\u1FD3' >= c) || (U'\u212A' <= c && U'\u212D' >= c) || (U'\u2135' <= c && U'\u2138' >= c) || (U'\u213C' <= c && U'\u213F' >= c) || (U'\u2185' <= c && U'\u2188' >= c) || (U'\u2CEB' <= c && U'\u2CEE' >= c) || (U'\uA78B' <= c && U'\uA78E' >= c) || (U'\uA807' <= c && U'\uA80A' >= c) || (U'\uA9B6' <= c && U'\uA9B9' >= c) || (U'\uAB5C' <= c && U'\uAB5F' >= c) || (U'\U00010A0C' <= c && U'\U00010A0F' >= c) || (U'\U00010A10' <= c && U'\U00010A13' >= c) || (U'\U00010D24' <= c && U'\U00010D27' >= c) || (U'\U000110B3' <= c && U'\U000110B6' >= c) || (U'\U000111C1' <= c && U'\U000111C4' >= c) || (U'\U0001128A' <= c && U'\U0001128D' >= c) || (U'\U00011341' <= c && U'\U00011344' >= c) || (U'\U00011447' <= c && U'\U0001144A' >= c) || (U'\U000114BB' <= c && U'\U000114BE' >= c) || (U'\U000115B2' <= c && U'\U000115B5' >= c) || (U'\U000115B8' <= c && U'\U000115BB' >= c) || (U'\U000115D8' <= c && U'\U000115DB' >= c) || (U'\U00011722' <= c && U'\U00011725' >= c) || (U'\U00011727' <= c && U'\U0001172A' >= c) || (U'\U00011A35' <= c && U'\U00011A38' >= c) || (U'\U00011A3B' <= c && U'\U00011A3E' >= c) || (U'\U00011A86' <= c && U'\U00011A89' >= c) || (U'\U00016B40' <= c && U'\U00016B43' >= c) || (U'\U0001D4A9' <= c && U'\U0001D4AC' >= c) || (U'\U0001D507' <= c && U'\U0001D50A' >= c) || (U'\U0001D53B' <= c && U'\U0001D53E' >= c) || (U'\U0001EE00' <= c && U'\U0001EE03' >= c) || (U'\U0001EE34' <= c && U'\U0001EE37' >= c) || (U'\U0001EE67' <= c && U'\U0001EE6A' >= c) || (U'\U0001EE74' <= c && U'\U0001EE77' >= c) || (U'\U0001EE79' <= c && U'\U0001EE7C' >= c) || U'\u00AA' == c || U'\u00B5' == c || U'\u00BA' == c || U'\u01BB' == c || U'\u0294' == c || U'\u02EC' == c || U'\u02EE' == c || U'\u0345' == c || U'\u0374' == c || U'\u037A' == c || (U'\u037B' <= c && U'\u037D' >= c) || U'\u037F' == c || U'\u0386' == c || (U'\u0388' <= c && U'\u038A' >= c) || U'\u038C' == c || U'\u0559' == c || U'\u05BF' == c || U'\u05C7' == c || U'\u0640' == c || U'\u0670' == c || U'\u06D5' == c || U'\u06ED' == c || (U'\u06FA' <= c && U'\u06FC' >= c) || U'\u06FF' == c || U'\u0710' == c || U'\u0711' == c || U'\u07B1' == c || U'\u07FA' == c || U'\u081A' == c || U'\u0824' == c || (U'\u0825' <= c && U'\u0827' >= c) || U'\u0828' == c || U'\u0903' == c || U'\u093A' == c || U'\u093B' == c || U'\u093D' == c || (U'\u093E' <= c && U'\u0940' >= c) || U'\u0950' == c || (U'\u0955' <= c && U'\u0957' >= c) || U'\u0971' == c || U'\u0981' == c || U'\u09B2' == c || U'\u09BD' == c || (U'\u09BE' <= c && U'\u09C0' >= c) || U'\u09CE' == c || U'\u09D7' == c || (U'\u09DF' <= c && U'\u09E1' >= c) || U'\u09FC' == c || U'\u0A03' == c || (U'\u0A3E' <= c && U'\u0A40' >= c) || U'\u0A51' == c || U'\u0A5E' == c || (U'\u0A72' <= c && U'\u0A74' >= c) || U'\u0A75' == c || U'\u0A83' == c || (U'\u0A8F' <= c && U'\u0A91' >= c) || U'\u0ABD' == c || (U'\u0ABE' <= c && U'\u0AC0' >= c) || U'\u0AC9' == c || U'\u0AD0' == c || U'\u0AF9' == c || (U'\u0AFA' <= c && U'\u0AFC' >= c) || U'\u0B01' == c || U'\u0B3D' == c || U'\u0B3E' == c || U'\u0B3F' == c || U'\u0B40' == c || U'\u0B56' == c || U'\u0B57' == c || (U'\u0B5F' <= c && U'\u0B61' >= c) || U'\u0B71' == c || U'\u0B82' == c || U'\u0B83' == c || (U'\u0B8E' <= c && U'\u0B90' >= c) || U'\u0B9C' == c || (U'\u0BA8' <= c && U'\u0BAA' >= c) || U'\u0BC0' == c || (U'\u0BC6' <= c && U'\u0BC8' >= c) || (U'\u0BCA' <= c && U'\u0BCC' >= c) || U'\u0BD0' == c || U'\u0BD7' == c || U'\u0C00' == c || (U'\u0C01' <= c && U'\u0C03' >= c) || (U'\u0C0E' <= c && U'\u0C10' >= c) || U'\u0C3D' == c || (U'\u0C3E' <= c && U'\u0C40' >= c) || (U'\u0C46' <= c && U'\u0C48' >= c) || (U'\u0C4A' <= c && U'\u0C4C' >= c) || (U'\u0C58' <= c && U'\u0C5A' >= c) || U'\u0C80' == c || U'\u0C81' == c || (U'\u0C8E' <= c && U'\u0C90' >= c) || U'\u0CBD' == c || U'\u0CBE' == c || U'\u0CBF' == c || U'\u0CC6' == c || U'\u0CCC' == c || U'\u0CDE' == c || (U'\u0D0E' <= c && U'\u0D10' >= c) || U'\u0D3D' == c || (U'\u0D3E' <= c && U'\u0D40' >= c) || (U'\u0D46' <= c && U'\u0D48' >= c) || (U'\u0D4A' <= c && U'\u0D4C' >= c) || U'\u0D4E' == c || (U'\u0D54' <= c && U'\u0D56' >= c) || U'\u0D57' == c || (U'\u0D5F' <= c && U'\u0D61' >= c) || U'\u0DBD' == c || (U'\u0DCF' <= c && U'\u0DD1' >= c) || (U'\u0DD2' <= c && U'\u0DD4' >= c) || U'\u0DD6' == c || U'\u0E31' == c || U'\u0E46' == c || U'\u0E4D' == c || U'\u0E84' == c || U'\u0E8A' == c || U'\u0E8D' == c || (U'\u0EA1' <= c && U'\u0EA3' >= c) || U'\u0EA5' == c || U'\u0EA7' == c || U'\u0EB1' == c || U'\u0EBD' == c || U'\u0EC6' == c || U'\u0ECD' == c || U'\u0F00' == c || U'\u0F7F' == c || U'\u1031' == c || U'\u1038' == c || U'\u103F' == c || (U'\u105E' <= c && U'\u1060' >= c) || U'\u1061' == c || U'\u1062' == c || (U'\u106E' <= c && U'\u1070' >= c) || U'\u1082' == c || U'\u108E' == c || U'\u109C' == c || U'\u109D' == c || U'\u10C7' == c || U'\u10CD' == c || U'\u10FC' == c || (U'\u10FD' <= c && U'\u10FF' >= c) || U'\u1258' == c || U'\u12C0' == c || U'\u135F' == c || (U'\u16EE' <= c && U'\u16F0' >= c) || (U'\u176E' <= c && U'\u1770' >= c) || U'\u17B6' == c || U'\u17C6' == c || U'\u17D7' == c || U'\u17DC' == c || U'\u1843' == c || U'\u18A9' == c || U'\u18AA' == c || (U'\u1920' <= c && U'\u1922' >= c) || (U'\u1929' <= c && U'\u192B' >= c) || U'\u1932' == c || U'\u1A1B' == c || U'\u1A55' == c || U'\u1A56' == c || U'\u1A57' == c || U'\u1A61' == c || U'\u1A62' == c || U'\u1AA7' == c || U'\u1B04' == c || U'\u1B35' == c || U'\u1B3B' == c || U'\u1B3C' == c || U'\u1B42' == c || U'\u1B43' == c || U'\u1B82' == c || U'\u1BA1' == c || U'\u1BE7' == c || (U'\u1BEA' <= c && U'\u1BEC' >= c) || U'\u1BED' == c || U'\u1BEE' == c || (U'\u1BEF' <= c && U'\u1BF1' >= c) || (U'\u1C4D' <= c && U'\u1C4F' >= c) || (U'\u1CBD' <= c && U'\u1CBF' >= c) || U'\u1D78' == c || U'\u1F59' == c || U'\u1F5B' == c || U'\u1F5D' == c || U'\u1FBE' == c || (U'\u1FC2' <= c && U'\u1FC4' >= c) || (U'\u1FF2' <= c && U'\u1FF4' >= c) || U'\u2071' == c || U'\u207F' == c || U'\u2102' == c || U'\u2107' == c || U'\u2115' == c || U'\u2124' == c || U'\u2126' == c || U'\u2128' == c || U'\u2139' == c || U'\u214E' == c || U'\u2D27' == c || U'\u2D2D' == c || U'\u2D6F' == c || U'\u2E2F' == c || U'\u3005' == c || U'\u3006' == c || U'\u3007' == c || (U'\u3038' <= c && U'\u303A' >= c) || U'\u303B' == c || U'\u303C' == c || U'\u309F' == c || (U'\u30FC' <= c && U'\u30FE' >= c) || U'\u30FF' == c || U'\uA015' == c || U'\uA60C' == c || U'\uA66E' == c || U'\uA67F' == c || U'\uA770' == c || U'\uA788' == c || U'\uA78F' == c || U'\uA7F7' == c || U'\uA7FA' == c || (U'\uA803' <= c && U'\uA805' >= c) || U'\uA827' == c || U'\uA8C5' == c || U'\uA8FB' == c || U'\uA952' == c || (U'\uA980' <= c && U'\uA982' >= c) || U'\uA983' == c || U'\uA9BC' == c || (U'\uA9BD' <= c && U'\uA9BF' >= c) || U'\uA9CF' == c || U'\uA9E6' == c || (U'\uAA40' <= c && U'\uAA42' >= c) || U'\uAA43' == c || U'\uAA4C' == c || U'\uAA4D' == c || U'\uAA70' == c || U'\uAA7A' == c || U'\uAAB0' == c || U'\uAAB1' == c || (U'\uAAB2' <= c && U'\uAAB4' >= c) || U'\uAABE' == c || U'\uAAC0' == c || U'\uAAC2' == c || U'\uAADD' == c || U'\uAAEB' == c || U'\uAAF2' == c || U'\uAAF5' == c || U'\uABE5' == c || U'\uABE8' == c || U'\uFB1D' == c || U'\uFB1E' == c || U'\uFB3E' == c || U'\uFF70' == c || (U'\uFFDA' <= c && U'\uFFDC' >= c) || U'\U00010341' == c || U'\U0001034A' == c || U'\U00010808' == c || U'\U0001083C' == c || U'\U00010A00' == c || (U'\U00010A01' <= c && U'\U00010A03' >= c) || (U'\U00010A15' <= c && U'\U00010A17' >= c) || U'\U00010F27' == c || U'\U00011000' == c || U'\U00011001' == c || U'\U00011002' == c || U'\U00011082' == c || (U'\U000110B0' <= c && U'\U000110B2' >= c) || (U'\U00011100' <= c && U'\U00011102' >= c) || U'\U0001112C' == c || U'\U00011144' == c || U'\U00011176' == c || U'\U00011182' == c || (U'\U000111B3' <= c && U'\U000111B5' >= c) || U'\U000111BF' == c || U'\U000111DA' == c || U'\U000111DC' == c || (U'\U0001122C' <= c && U'\U0001122E' >= c) || (U'\U0001122F' <= c && U'\U00011231' >= c) || U'\U00011234' == c || U'\U00011237' == c || U'\U0001123E' == c || U'\U00011288' == c || U'\U000112DF' == c || (U'\U000112E0' <= c && U'\U000112E2' >= c) || U'\U0001133D' == c || U'\U00011340' == c || U'\U00011350' == c || U'\U00011357' == c || (U'\U00011435' <= c && U'\U00011437' >= c) || U'\U00011445' == c || (U'\U000114B0' <= c && U'\U000114B2' >= c) || U'\U000114B9' == c || U'\U000114BA' == c || U'\U000114C1' == c || U'\U000114C7' == c || (U'\U000115AF' <= c && U'\U000115B1' >= c) || U'\U000115BE' == c || (U'\U00011630' <= c && U'\U00011632' >= c) || U'\U0001163D' == c || U'\U0001163E' == c || U'\U00011640' == c || U'\U00011644' == c || U'\U000116AB' == c || U'\U000116AC' == c || U'\U000116AD' == c || (U'\U0001171D' <= c && U'\U0001171F' >= c) || U'\U00011726' == c || (U'\U0001182C' <= c && U'\U0001182E' >= c) || U'\U00011838' == c || U'\U000118FF' == c || U'\U00011A00' == c || U'\U00011A39' == c || U'\U00011A3A' == c || U'\U00011A50' == c || (U'\U00011A59' <= c && U'\U00011A5B' >= c) || U'\U00011A97' == c || U'\U00011A9D' == c || U'\U00011C2F' == c || U'\U00011C3E' == c || U'\U00011C40' == c || U'\U00011CA9' == c || U'\U00011CB1' == c || U'\U00011CB4' == c || U'\U00011D3A' == c || (U'\U00011D3F' <= c && U'\U00011D41' >= c) || U'\U00011D43' == c || U'\U00011D46' == c || U'\U00011D47' == c || U'\U00011D95' == c || U'\U00011D96' == c || U'\U00011D98' == c || U'\U00016F50' == c || U'\U0001BC9E' == c || U'\U0001D4A2' == c || U'\U0001D4BB' == c || U'\U0001D546' == c || U'\U0001E947' == c || U'\U0001EE24' == c || U'\U0001EE27' == c || U'\U0001EE39' == c || U'\U0001EE3B' == c || U'\U0001EE42' == c || U'\U0001EE47' == c || U'\U0001EE49' == c || U'\U0001EE4B' == c || (U'\U0001EE4D' <= c && U'\U0001EE4F' >= c) || U'\U0001EE54' == c || U'\U0001EE57' == c || U'\U0001EE59' == c || U'\U0001EE5B' == c || U'\U0001EE5D' == c || U'\U0001EE5F' == c || U'\U0001EE64' == c || U'\U0001EE7E' == c || (U'\U0001EEA1' <= c && U'\U0001EEA3' >= c) || U'\u0376' == c || U'\u0377' == c || U'\u05C1' == c || U'\u05C2' == c || U'\u05C4' == c || U'\u05C5' == c || U'\u066E' == c || U'\u066F' == c || U'\u06E5' == c || U'\u06E6' == c || U'\u06E7' == c || U'\u06E8' == c || U'\u06EE' == c || U'\u06EF' == c || U'\u07F4' == c || U'\u07F5' == c || U'\u0816' == c || U'\u0817' == c || U'\u094E' == c || U'\u094F' == c || U'\u0962' == c || U'\u0963' == c || U'\u0982' == c || U'\u0983' == c || U'\u098F' == c || U'\u0990' == c || U'\u09C7' == c || U'\u09C8' == c || U'\u09CB' == c || U'\u09CC' == c || U'\u09DC' == c || U'\u09DD' == c || U'\u09E2' == c || U'\u09E3' == c || U'\u09F0' == c || U'\u09F1' == c || U'\u0A01' == c || U'\u0A02' == c || U'\u0A0F' == c || U'\u0A10' == c || U'\u0A32' == c || U'\u0A33' == c || U'\u0A35' == c || U'\u0A36' == c || U'\u0A38' == c || U'\u0A39' == c || U'\u0A41' == c || U'\u0A42' == c || U'\u0A47' == c || U'\u0A48' == c || U'\u0A4B' == c || U'\u0A4C' == c || U'\u0A70' == c || U'\u0A71' == c || U'\u0A81' == c || U'\u0A82' == c || U'\u0AB2' == c || U'\u0AB3' == c || U'\u0AC7' == c || U'\u0AC8' == c || U'\u0ACB' == c || U'\u0ACC' == c || U'\u0AE0' == c || U'\u0AE1' == c || U'\u0AE2' == c || U'\u0AE3' == c || U'\u0B02' == c || U'\u0B03' == c || U'\u0B0F' == c || U'\u0B10' == c || U'\u0B32' == c || U'\u0B33' == c || U'\u0B47' == c || U'\u0B48' == c || U'\u0B4B' == c || U'\u0B4C' == c || U'\u0B5C' == c || U'\u0B5D' == c || U'\u0B62' == c || U'\u0B63' == c || U'\u0B99' == c || U'\u0B9A' == c || U'\u0B9E' == c || U'\u0B9F' == c || U'\u0BA3' == c || U'\u0BA4' == c || U'\u0BBE' == c || U'\u0BBF' == c || U'\u0BC1' == c || U'\u0BC2' == c || U'\u0C55' == c || U'\u0C56' == c || U'\u0C60' == c || U'\u0C61' == c || U'\u0C62' == c || U'\u0C63' == c || U'\u0C82' == c || U'\u0C83' == c || U'\u0CC7' == c || U'\u0CC8' == c || U'\u0CCA' == c || U'\u0CCB' == c || U'\u0CD5' == c || U'\u0CD6' == c || U'\u0CE0' == c || U'\u0CE1' == c || U'\u0CE2' == c || U'\u0CE3' == c || U'\u0CF1' == c || U'\u0CF2' == c || U'\u0D00' == c || U'\u0D01' == c || U'\u0D02' == c || U'\u0D03' == c || U'\u0D62' == c || U'\u0D63' == c || U'\u0D82' == c || U'\u0D83' == c || U'\u0DF2' == c || U'\u0DF3' == c || U'\u0E32' == c || U'\u0E33' == c || U'\u0E81' == c || U'\u0E82' == c || U'\u0E87' == c || U'\u0E88' == c || U'\u0EAA' == c || U'\u0EAB' == c || U'\u0EB2' == c || U'\u0EB3' == c || U'\u0EBB' == c || U'\u0EBC' == c || U'\u0F80' == c || U'\u0F81' == c || U'\u102B' == c || U'\u102C' == c || U'\u103B' == c || U'\u103C' == c || U'\u103D' == c || U'\u103E' == c || U'\u1056' == c || U'\u1057' == c || U'\u1058' == c || U'\u1059' == c || U'\u1065' == c || U'\u1066' == c || U'\u1067' == c || U'\u1068' == c || U'\u1083' == c || U'\u1084' == c || U'\u1085' == c || U'\u1086' == c || U'\u1712' == c || U'\u1713' == c || U'\u1732' == c || U'\u1733' == c || U'\u1752' == c || U'\u1753' == c || U'\u1772' == c || U'\u1773' == c || U'\u17C7' == c || U'\u17C8' == c || U'\u1885' == c || U'\u1886' == c || U'\u1927' == c || U'\u1928' == c || U'\u1930' == c || U'\u1931' == c || U'\u1A17' == c || U'\u1A18' == c || U'\u1A19' == c || U'\u1A1A' == c || U'\u1A63' == c || U'\u1A64' == c || U'\u1A73' == c || U'\u1A74' == c || U'\u1B80' == c || U'\u1B81' == c || U'\u1BA6' == c || U'\u1BA7' == c || U'\u1BA8' == c || U'\u1BA9' == c || U'\u1BAC' == c || U'\u1BAD' == c || U'\u1BAE' == c || U'\u1BAF' == c || U'\u1BE8' == c || U'\u1BE9' == c || U'\u1C34' == c || U'\u1C35' == c || U'\u1CF2' == c || U'\u1CF3' == c || U'\u1CF5' == c || U'\u1CF6' == c || U'\u2183' == c || U'\u2184' == c || U'\u2C7C' == c || U'\u2C7D' == c || U'\u2CF2' == c || U'\u2CF3' == c || U'\u309D' == c || U'\u309E' == c || U'\uA62A' == c || U'\uA62B' == c || U'\uA69C' == c || U'\uA69D' == c || U'\uA69E' == c || U'\uA69F' == c || U'\uA7F8' == c || U'\uA7F9' == c || U'\uA823' == c || U'\uA824' == c || U'\uA825' == c || U'\uA826' == c || U'\uA880' == c || U'\uA881' == c || U'\uA8FD' == c || U'\uA8FE' == c || U'\uA9B4' == c || U'\uA9B5' == c || U'\uA9BA' == c || U'\uA9BB' == c || U'\uAA2F' == c || U'\uAA30' == c || U'\uAA31' == c || U'\uAA32' == c || U'\uAA33' == c || U'\uAA34' == c || U'\uAA35' == c || U'\uAA36' == c || U'\uAAB5' == c || U'\uAAB6' == c || U'\uAAB7' == c || U'\uAAB8' == c || U'\uAADB' == c || U'\uAADC' == c || U'\uAAEC' == c || U'\uAAED' == c || U'\uAAEE' == c || U'\uAAEF' == c || U'\uAAF3' == c || U'\uAAF4' == c || U'\uABE3' == c || U'\uABE4' == c || U'\uABE6' == c || U'\uABE7' == c || U'\uABE9' == c || U'\uABEA' == c || U'\uFB40' == c || U'\uFB41' == c || U'\uFB43' == c || U'\uFB44' == c || U'\uFF9E' == c || U'\uFF9F' == c || U'\U0001003C' == c || U'\U0001003D' == c || U'\U00010837' == c || U'\U00010838' == c || U'\U000108F4' == c || U'\U000108F5' == c || U'\U000109BE' == c || U'\U000109BF' == c || U'\U00010A05' == c || U'\U00010A06' == c || U'\U000110B7' == c || U'\U000110B8' == c || U'\U00011145' == c || U'\U00011146' == c || U'\U00011180' == c || U'\U00011181' == c || U'\U00011232' == c || U'\U00011233' == c || U'\U00011300' == c || U'\U00011301' == c || U'\U00011302' == c || U'\U00011303' == c || U'\U0001130F' == c || U'\U00011310' == c || U'\U00011332' == c || U'\U00011333' == c || U'\U0001133E' == c || U'\U0001133F' == c || U'\U00011347' == c || U'\U00011348' == c || U'\U0001134B' == c || U'\U0001134C' == c || U'\U00011362' == c || U'\U00011363' == c || U'\U00011440' == c || U'\U00011441' == c || U'\U00011443' == c || U'\U00011444' == c || U'\U000114BF' == c || U'\U000114C0' == c || U'\U000114C4' == c || U'\U000114C5' == c || U'\U000115BC' == c || U'\U000115BD' == c || U'\U000115DC' == c || U'\U000115DD' == c || U'\U0001163B' == c || U'\U0001163C' == c || U'\U000116AE' == c || U'\U000116AF' == c || U'\U00011720' == c || U'\U00011721' == c || U'\U00011A57' == c || U'\U00011A58' == c || U'\U00011CB2' == c || U'\U00011CB3' == c || U'\U00011CB5' == c || U'\U00011CB6' == c || U'\U00011D08' == c || U'\U00011D09' == c || U'\U00011D3C' == c || U'\U00011D3D' == c || U'\U00011D67' == c || U'\U00011D68' == c || U'\U00011D90' == c || U'\U00011D91' == c || U'\U00011D93' == c || U'\U00011D94' == c || U'\U00011EF3' == c || U'\U00011EF4' == c || U'\U00011EF5' == c || U'\U00011EF6' == c || U'\U00016FE0' == c || U'\U00016FE1' == c || U'\U0001D49E' == c || U'\U0001D49F' == c || U'\U0001D4A5' == c || U'\U0001D4A6' == c || U'\U0001E023' == c || U'\U0001E024' == c || U'\U0001EE21' == c || U'\U0001EE22' == c || U'\U0001EE51' == c || U'\U0001EE52' == c || U'\U0001EE61' == c || U'\U0001EE62' == c;
}

bool WordBreak::_gen_is_script_katakana(char32_t c) {
  return (U'\u30A1' <= c && U'\u30FA' >= c) || (U'\u3300' <= c && U'\u3357' >= c) || (U'\u32D0' <= c && U'\u32FE' >= c) || (U'\uFF71' <= c && U'\uFF9D' >= c) || (U'\u31F0' <= c && U'\u31FF' >= c) || (U'\uFF66' <= c && U'\uFF6F' >= c) || U'\u30FF' == c || U'\U0001B000' == c || U'\u30FD' == c || U'\u30FE' == c;
}

bool WordBreak::_gen_is_script_hiragana(char32_t c) {
  return (U'\U0001B001' <= c && U'\U0001B11E' >= c) || (U'\u3041' <= c && U'\u3096' >= c) || U'\u309F' == c || U'\U0001F200' == c || U'\u309D' == c || U'\u309E' == c;
}

bool WordBreak::_gen_is_script_hebrew(char32_t c) {
  return (U'\u0591' <= c && U'\u05BD' >= c) || (U'\u05D0' <= c && U'\u05EA' >= c) || (U'\uFB2A' <= c && U'\uFB36' >= c) || (U'\uFB1F' <= c && U'\uFB28' >= c) || (U'\uFB46' <= c && U'\uFB4F' >= c) || (U'\uFB38' <= c && U'\uFB3C' >= c) || (U'\u05EF' <= c && U'\u05F2' >= c) || U'\u05BE' == c || U'\u05BF' == c || U'\u05C0' == c || U'\u05C3' == c || U'\u05C6' == c || U'\u05C7' == c || U'\uFB1D' == c || U'\uFB1E' == c || U'\uFB29' == c || U'\uFB3E' == c || U'\u05C1' == c || U'\u05C2' == c || U'\u05C4' == c || U'\u05C5' == c || U'\u05F3' == c || U'\u05F4' == c || U'\uFB40' == c || U'\uFB41' == c || U'\uFB43' == c || U'\uFB44' == c;
}

bool WordBreak::_gen_is_ideographic(char32_t c) {
  return (U'\U00020000' <= c && U'\U0002A6D6' >= c) || (U'\u4E00' <= c && U'\u9FEF' >= c) || (U'\U0002CEB0' <= c && U'\U0002EBE0' >= c) || (U'\u3400' <= c && U'\u4DB5' >= c) || (U'\U00017000' <= c && U'\U000187F1' >= c) || (U'\U0002B820' <= c && U'\U0002CEA1' >= c) || (U'\U0002A700' <= c && U'\U0002B734' >= c) || (U'\U00018800' <= c && U'\U00018AF2' >= c) || (U'\U0002F800' <= c && U'\U0002FA1D' >= c) || (U'\U0001B170' <= c && U'\U0001B2FB' >= c) || (U'\uF900' <= c && U'\uFA6D' >= c) || (U'\U0002B740' <= c && U'\U0002B81D' >= c) || (U'\uFA70' <= c && U'\uFAD9' >= c) || (U'\u3021' <= c && U'\u3029' >= c) || U'\u3006' == c || U'\u3007' == c || (U'\u3038' <= c && U'\u303A' >= c);
}

bool WordBreak::_gen_is_east_asian_width_full(char32_t c) {
  return (U'\uFF21' <= c && U'\uFF3A' >= c) || (U'\uFF41' <= c && U'\uFF5A' >= c) || (U'\uFF10' <= c && U'\uFF19' >= c) || U'\u3000' == c || (U'\uFF01' <= c && U'\uFF03' >= c) || U'\uFF04' == c || (U'\uFF05' <= c && U'\uFF07' >= c) || U'\uFF08' == c || U'\uFF09' == c || U'\uFF0A' == c || U'\uFF0B' == c || U'\uFF0C' == c || U'\uFF0D' == c || (U'\uFF1C' <= c && U'\uFF1E' >= c) || U'\uFF3B' == c || U'\uFF3C' == c || U'\uFF3D' == c || U'\uFF3E' == c || U'\uFF3F' == c || U'\uFF40' == c || U'\uFF5B' == c || U'\uFF5C' == c || U'\uFF5D' == c || U'\uFF5E' == c || U'\uFF5F' == c || U'\uFF60' == c || U'\uFFE2' == c || U'\uFFE3' == c || U'\uFFE4' == c || U'\uFF0E' == c || U'\uFF0F' == c || U'\uFF1A' == c || U'\uFF1B' == c || U'\uFF1F' == c || U'\uFF20' == c || U'\uFFE0' == c || U'\uFFE1' == c || U'\uFFE5' == c || U'\uFFE6' == c;
}
