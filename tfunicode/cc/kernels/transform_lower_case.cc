#include "tensorflow/core/framework/op.h"
#include "tfunicode/cc/lib/transform_base.h"
#include <unicode/unistr.h>

using icu::UnicodeString;


class TransformLowerCaseOp : public TransformBaseOp {
 public:
  explicit TransformLowerCaseOp(OpKernelConstruction* ctx) : TransformBaseOp(ctx) {}

 protected:
  void transform(UnicodeString &item, UErrorCode &error) {
    item.toLower();
  }
};

REGISTER_KERNEL_BUILDER(Name("TransformLowerCase").Device(DEVICE_CPU), TransformLowerCaseOp);
