#pragma once
#include <string>

using namespace std;


class WordBreak {
 public:
  static bool IsBreak(const u32string &source, const int position, const bool extended);

 protected:
  static inline char32_t CharAt(const u32string &source, const int position);
  static inline int Skip_EFZ(const u32string &source, const int start, const int direction, const bool zwj);

  static inline bool CR(char32_t c);
  static inline bool LF(char32_t c);
  static inline bool Newline(char32_t c);
  static inline bool Extend(char32_t c);
  static inline bool ZWJ(char32_t c);
  static inline bool Regional_Indicator(char32_t c);
  static inline bool Format(char32_t c);
  static inline bool Katakana(char32_t c);
  static inline bool Hebrew_Letter(char32_t c);
  static inline bool ALetter(char32_t c);
  static inline bool Single_Quote(char32_t c);
  static inline bool Double_Quote(char32_t c);
  static inline bool MidNumLet(char32_t c);
  static inline bool MidLetter(char32_t c);
  static inline bool MidNum(char32_t c);
  static inline bool Numeric(char32_t c);
  static inline bool ExtendNumLet(char32_t c);
  static inline bool WSegSpace(char32_t c);
  static inline bool AHLetter(char32_t c);
  static inline bool MidNumLetQ(char32_t c);
  static inline bool Grapheme_Extend(char32_t c);
  static inline bool Emoji_Modifier(char32_t c);
  static inline bool Alphabetic(char32_t c);
  static inline bool Ideographic(char32_t c);
  static inline bool Linebreak_ComplexContext(char32_t c);
  static inline bool Hiragana(char32_t c);
  static inline bool Linebreak_Glue(char32_t c);
  static inline bool Extended_Pictographic(char32_t c);
  static inline bool Linebreak_InfixNumeric(char32_t c);
  static inline bool Linebreak_Numeric(char32_t c);
  static inline bool MidNumLetQExt(char32_t c);

 private:
  // Methods below generated with tools/word_break_groups.py
  static inline bool _gen_is_extended_pictographic(char32_t c);
  static inline bool _gen_is_other_grapheme_extend(char32_t c);
  static inline bool _gen_is_alphabetic(char32_t c);
  static inline bool _gen_is_script_katakana(char32_t c);
  static inline bool _gen_is_script_hiragana(char32_t c);
  static inline bool _gen_is_script_hebrew(char32_t c);
  static inline bool _gen_is_ideographic(char32_t c);
  static inline bool _gen_is_east_asian_width_full(char32_t c);
};
