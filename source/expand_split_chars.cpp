#ifndef TF_UC_OPS_EXPAND_SPLIT_CHARS
#define TF_UC_OPS_EXPAND_SPLIT_CHARS

#include <unicode/unistr.h>
#include "tensorflow/core/framework/op.h"
#include "expand_base.cpp"

using icu::UnicodeString;


REGISTER_OP("ExpandSplitChars")
  .Input("source: string")
  .Output("indices: int64")
  .Output("values: string")
  .Output("shape: int64")
  .SetShapeFn(ExpandBaseShape)
  .SetIsStateful();


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

#endif
