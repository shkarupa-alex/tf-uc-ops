#ifndef TF_UC_OPS_EXPAND_SPLIT_CHARS
#define TF_UC_OPS_EXPAND_SPLIT_CHARS

#include "expand_base.cpp"

REGISTER_OP("ExpandSplitChars")
  .Input("source: string")
  .Attr("default: string")
  .Output("result: string")
  .SetShapeFn([](shape_inference::InferenceContext* c) {
    shape_inference::ShapeHandle input = c->input(0);
    shape_inference::ShapeHandle append = c->Vector(shape_inference::InferenceContext::kUnknownDim);

    shape_inference::ShapeHandle output;
    TF_RETURN_IF_ERROR(c->Concatenate(input, append, &output));

    c->set_output(0, output);

    return Status::OK();
  })
  .SetIsStateful();


class ExpandSplitCharsOp : public ExpandBaseOp {
 public:
  explicit ExpandSplitCharsOp(OpKernelConstruction* ctx) : ExpandBaseOp(ctx) {}

 private:
  void expand(const UnicodeString &source, std::vector<UnicodeString> &target, UErrorCode &error) {
    if (source.length() < 2) {
      target.push_back(source);
      return;
    }

    // Split chars
    for (int32_t pos = 0; pos < source.length(); pos++) {
      UnicodeString word = UnicodeString(source, pos, 1);

      target.push_back(word);
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("ExpandSplitChars").Device(DEVICE_CPU), ExpandSplitCharsOp);

#endif
