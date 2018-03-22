#ifndef TF_UC_OPS_EXPAND_CHAR_NGRAMS
#define TF_UC_OPS_EXPAND_CHAR_NGRAMS

#include "expand_base.cpp"

REGISTER_OP("ExpandCharNgrams")
  .Input("source: string")
  .Attr("minn: int")
  .Attr("maxn: int")
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


class ExpandCharNgramsOp : public ExpandBaseOp {
 public:
  explicit ExpandCharNgramsOp(OpKernelConstruction* ctx) : ExpandBaseOp(ctx) {
    // Prepare attrs
    OP_REQUIRES_OK(ctx, ctx->GetAttr("minn", &minn));
    OP_REQUIRES_OK(ctx, ctx->GetAttr("maxn", &maxn));
    OP_REQUIRES(ctx, minn > 0, errors::InvalidArgument("minn should be above 0"));
    OP_REQUIRES(ctx, maxn > 0, errors::InvalidArgument("maxn should be above 0"));
    OP_REQUIRES(ctx, maxn >= minn, errors::InvalidArgument("maxn should be above or equal minn"));
}

 private:
  int minn;
  int maxn;

  void expand(const UnicodeString &source, std::vector<UnicodeString> &target, UErrorCode &error) {
    if (source.length() < minn) {
        target.push_back(UnicodeString::fromUTF8(default_value));
        return;
    }

    // Split ngrams
    for (int32_t n = minn; n <= maxn; n++) {
      for (int32_t pos = 0; pos <= source.length() - n; pos++) {
        UnicodeString ngram = UnicodeString(source, pos, n);

        target.push_back(ngram);
      }
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("ExpandCharNgrams").Device(DEVICE_CPU), ExpandCharNgramsOp);

#endif
