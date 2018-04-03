#include <unicode/uversion.h>

#if U_ICU_VERSION_MAJOR_NUM < 60
#error Required ICU version >= 60.0
#endif


#include "transform_normalize_unicode.cpp"
#include "transform_lower_case.cpp"
#include "transform_upper_case.cpp"
#include "transform_zero_digits.cpp"
#include "transform_wrap_with.cpp"
#include "expand_split_words.cpp"
#include "expand_split_chars.cpp"
#include "expand_char_ngrams.cpp"
