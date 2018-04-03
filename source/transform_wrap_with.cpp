#ifndef TF_UC_OPS_TRANSFORM_WRAP_WITH
#define TF_UC_OPS_TRANSFORM_WRAP_WITH

#include <unicode/unistr.h>
#include "tensorflow/core/framework/op.h"
#include "transform_base.cpp"

using icu::UnicodeString;


REGISTER_OP("TransformWrapWith")
  .Input("source: string")
  .Attr("left: string")
  .Attr("right: string")
  .Output("result: string")
  .SetShapeFn(TransformBaseShape)
  .SetIsStateful();


class TransformWrapWithOp : public TransformBaseOp {
 public:
  explicit TransformWrapWithOp(OpKernelConstruction* ctx) : TransformBaseOp(ctx) {
    // Prepare attrs
    string _left;
    OP_REQUIRES_OK(ctx, ctx->GetAttr("left", &_left));
    left = UnicodeString::fromUTF8(_left);

    string _right;
    OP_REQUIRES_OK(ctx, ctx->GetAttr("right", &_right));
    right = UnicodeString::fromUTF8(_right);
  }

 private:
  UnicodeString left;
  UnicodeString right;

 protected:
  void transform(UnicodeString &item, UErrorCode &error) {
    item = left + item + right;
  }
};

REGISTER_KERNEL_BUILDER(Name("TransformWrapWith").Device(DEVICE_CPU), TransformWrapWithOp);

#endif