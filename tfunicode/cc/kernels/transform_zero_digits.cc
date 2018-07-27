#include "tensorflow/core/framework/op.h"
#include "tfunicode/cc/lib/transform_base.h"
#include <unicode/unistr.h>
#include <unicode/uchar.h>

using icu::UnicodeString;


class TransformZeroDigitsOp : public TransformBaseOp {
 public:
  explicit TransformZeroDigitsOp(OpKernelConstruction* ctx) : TransformBaseOp(ctx) {}

 protected:
  const UChar zero = 48; // Unicode 0

  void transform(UnicodeString &item, UErrorCode &error) {
    for (int32_t pos = 0; pos < item.length(); pos++) {
      if (u_isdigit(item.charAt(pos))) {
        item.replace(pos, 1, zero);
      }
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("TransformZeroDigits").Device(DEVICE_CPU), TransformZeroDigitsOp);
