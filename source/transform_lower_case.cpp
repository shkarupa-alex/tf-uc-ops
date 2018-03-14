#ifndef TF_UC_OPS_TRANSFORM_LOWER_CASE
#define TF_UC_OPS_TRANSFORM_LOWER_CASE

#include "tensorflow/core/framework/common_shape_fns.h"
#include "transform_base.cpp"

REGISTER_OP("TransformLowerCase")
  .Input("source: string")
  .Output("result: string")
  .SetShapeFn(shape_inference::UnchangedShape)
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