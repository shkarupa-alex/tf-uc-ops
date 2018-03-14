#ifndef TF_UC_OPS_TRANSFORM_ZERO_DIGITS
#define TF_UC_OPS_TRANSFORM_ZERO_DIGITS

#include "tensorflow/core/framework/common_shape_fns.h"
#include "transform_base.cpp"

REGISTER_OP("TransformZeroDigits")
  .Input("source: string")
  .Output("result: string")
  .SetShapeFn(shape_inference::UnchangedShape)
  .SetIsStateful();


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

#endif