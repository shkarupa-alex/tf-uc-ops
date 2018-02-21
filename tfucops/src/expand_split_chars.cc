#ifndef TF_UC_OPS_SPLIT_CHARS
#define TF_UC_OPS_SPLIT_CHARS

#include "expand_base.cc"
#include <unicode/brkiter.h>

REGISTER_OP("SplitChars")
  .Input("source: string")
  .Attr("default_value: string")
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


class SplitCharsOp : public ExpandBaseOp {
 public:
  explicit SplitCharsOp(OpKernelConstruction* ctx) : ExpandBaseOp(ctx) {}

 private:
  void expand(const UnicodeString &source, std::vector<UnicodeString> &target, UErrorCode &error) {
    if (2 > source.length()) {
      target.push_back(source);
      return;
    }

    // Split chars
    for (int32_t pos = 0; pos < source.length(); pos++) {
      UChar unit = source.charAt(pos);
      UnicodeString word = UnicodeString(source, pos, 1);

      target.push_back(word);
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("SplitChars").Device(DEVICE_CPU), SplitCharsOp);

#endif
