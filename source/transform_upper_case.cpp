#ifndef TF_UC_OPS_TRANSFORM_UPPER_CASE
#define TF_UC_OPS_TRANSFORM_UPPER_CASE

#include "tensorflow/core/framework/common_shape_fns.h"
#include "transform_base.cpp"

REGISTER_OP("TransformUpperCase")
  .Input("source: string")
  .Output("result: string")
  .SetShapeFn(shape_inference::UnchangedShape)
  .SetIsStateful();


class TransformUpperCaseOp : public TransformBaseOp {
 public:
  explicit TransformUpperCaseOp(OpKernelConstruction* ctx) : TransformBaseOp(ctx) {}

 protected:
  void transform(UnicodeString &item, UErrorCode &error) {
    item.toUpper();
  }
};

REGISTER_KERNEL_BUILDER(Name("TransformUpperCase").Device(DEVICE_CPU), TransformUpperCaseOp);

#endif