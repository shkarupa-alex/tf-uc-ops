#ifndef TF_UC_OPS_LOWERCASE_UNICODE
#define TF_UC_OPS_LOWERCASE_UNICODE

#include "transform_base.cc"

REGISTER_OP("LowercaseUnicode")
  .Input("source: string")
  .Attr("form: string")
  .Output("result: string")
  .SetShapeFn([](shape_inference::InferenceContext* c) {
    c->set_output(0, c->input(0));

    return Status::OK();
  })
  .SetIsStateful();


class LowercaseUnicodeOp : public TransformBaseOp {
 public:
  explicit LowercaseUnicodeOp(OpKernelConstruction* ctx) : TransformBaseOp(ctx) {}

 protected:
  void transform(UnicodeString &target, UErrorCode &error) {
    target.toLower();
  }
};

REGISTER_KERNEL_BUILDER(Name("LowercaseUnicode").Device(DEVICE_CPU), LowercaseUnicodeOp);

#endif