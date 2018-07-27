#include "tensorflow/core/framework/op.h"
#include "tfunicode/cc/lib/transform_base.h"
#include <unicode/unistr.h>

using icu::UnicodeString;


class TransformUpperCaseOp : public TransformBaseOp {
 public:
  explicit TransformUpperCaseOp(OpKernelConstruction* ctx) : TransformBaseOp(ctx) {}

 protected:
  void transform(UnicodeString &item, UErrorCode &error) {
    item.toUpper();
  }
};

REGISTER_KERNEL_BUILDER(Name("TransformUpperCase").Device(DEVICE_CPU), TransformUpperCaseOp);
