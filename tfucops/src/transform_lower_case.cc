#ifndef TF_UC_OPS_LOWERCASE_UNICODE
#define TF_UC_OPS_LOWERCASE_UNICODE

#include "tensorflow/core/framework/common_shape_fns.h"
#include "transform_base.cc"

REGISTER_OP("LowercaseUnicode")
  .Input("source: string")
  .Output("result: string")
  .SetShapeFn(shape_inference::UnchangedShape)
  .SetIsStateful();


class LowercaseUnicodeOp : public TransformBaseOp {
 public:
  explicit LowercaseUnicodeOp(OpKernelConstruction* ctx) : TransformBaseOp(ctx) {}

 protected:
  void transform(UnicodeString &item, UErrorCode &error) {
    item.toLower();
  }
};

REGISTER_KERNEL_BUILDER(Name("LowercaseUnicode").Device(DEVICE_CPU), LowercaseUnicodeOp);

#endif