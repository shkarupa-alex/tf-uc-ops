#ifndef TF_UC_OPS_EXPAND_CHAR_NGRAMS
#define TF_UC_OPS_EXPAND_CHAR_NGRAMS

#include "expand_base.cpp"

REGISTER_OP("ExpandCharNgrams")
  .Input("source: string")
  .Attr("minn: int")
  .Attr("maxn: int")
  .Attr("itself: {'ASIS', 'NEVER', 'ALWAYS'}")
  .Output("indices: int64")
  .Output("values: string")
  .Output("shape: int64")
  .SetShapeFn(ExpandBaseShape)
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

    string itself_value;
    OP_REQUIRES_OK(ctx, ctx->GetAttr("itself", &itself_value));
    std::transform(itself_value.begin(), itself_value.end(), itself_value.begin(), ::toupper);
    if("ASIS" == itself_value) {
      itself = 0;
    } else if ("NEVER" == itself_value) {
      itself = -1;
    } else if ("ALWAYS" == itself_value) {
      itself = 1;
    } else {
      OP_REQUIRES(ctx, false, errors::InvalidArgument("unknown itself value"));
    }
}

 private:
  int minn;
  int maxn;
  int itself;

  void expand(const UnicodeString &source, std::vector<UnicodeString> &target, UErrorCode &error) {
    // Split ngrams
    for (int32_t n = minn; n <= maxn; n++) {
      if (-1 == itself && source.length() == n)
        continue;

      for (int32_t pos = 0; pos <= source.length() - n; pos++) {
        UnicodeString ngram = UnicodeString(source, pos, n);

        target.push_back(ngram);
      }
    }

    if (1 == itself && (source.length() < minn || source.length() > maxn)) {
      target.push_back(source);
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("ExpandCharNgrams").Device(DEVICE_CPU), ExpandCharNgramsOp);

#endif
