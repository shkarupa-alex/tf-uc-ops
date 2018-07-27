#include "tensorflow/core/framework/op.h"
#include "tfunicode/cc/lib/expand_base.h"
#include <unicode/unistr.h>

using icu::UnicodeString;


class ExpandSplitCharsOp : public ExpandBaseOp {
 public:
  explicit ExpandSplitCharsOp(OpKernelConstruction* ctx) : ExpandBaseOp(ctx) {}

 private:
  void expand(const UnicodeString &source, std::vector<UnicodeString> &target, UErrorCode &error) {
    if (source.length() == 0) {
      return;
    }

    if (source.length() == 1) {
      target.push_back(source);
      return;
    }

    // Split chars
    for (int32_t pos = 0; pos < source.length(); pos++) {
      UnicodeString word = UnicodeString(source, pos, 1);

      target.push_back(word);
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("ExpandSplitChars").Device(DEVICE_CPU), ExpandSplitCharsOp);
