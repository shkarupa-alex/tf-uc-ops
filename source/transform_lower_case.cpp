#ifndef TF_UC_OPS_TRANSFORM_LOWER_CASE
#define TF_UC_OPS_TRANSFORM_LOWER_CASE

#include <unicode/unistr.h>
#include "tensorflow/core/framework/op.h"
#include "transform_base.cpp"

using icu::UnicodeString;


REGISTER_OP("TransformLowerCase")
  .Input("source: string")
  .Output("result: string")
  .SetShapeFn(TransformBaseShape)
  .SetIsStateful();


class TransformLowerCaseOp : public TransformBaseOp {
 public:
  explicit TransformLowerCaseOp(OpKernelConstruction* ctx) : TransformBaseOp(ctx) {}

 protected:
  void transform(UnicodeString &item, UErrorCode &error) {
    item.toLower();
  }
};

REGISTER_KERNEL_BUILDER(Name("TransformLowerCase").Device(DEVICE_CPU), TransformLowerCaseOp);

#endif