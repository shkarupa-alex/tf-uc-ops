#include "tfunicode/cc/lib/expand_base.h"

using namespace std;


class ExpandCharNgramsOp : public ExpandBaseOp {
 public:
  explicit ExpandCharNgramsOp(OpKernelConstruction* ctx) : ExpandBaseOp(ctx) {
    // Prepare attrs
    OP_REQUIRES_OK(ctx, ctx->GetAttr("minn", &minn));
    OP_REQUIRES_OK(ctx, ctx->GetAttr("maxn", &maxn));
    OP_REQUIRES(ctx, minn > 0, errors::InvalidArgument("minn should be above 0"));
    OP_REQUIRES(ctx, maxn >= minn, errors::InvalidArgument("maxn should be above or equal minn"));

    string itself_value;
    OP_REQUIRES_OK(ctx, ctx->GetAttr("itself", &itself_value));
    std::transform(itself_value.begin(), itself_value.end(), itself_value.begin(), ::toupper);
    if ("NEVER" == itself_value) {
      itself = NgramItself::NEVER;
    } else if ("ALWAYS" == itself_value) {
      itself = NgramItself::ALWAYS;
    } else {
      itself = NgramItself::ASIS;
    }
  }

 private:
  enum class NgramItself { ASIS, NEVER, ALWAYS };

  int minn;
  int maxn;
  NgramItself itself;

  inline void expand(const u32string &source, std::vector<u32string> &target) {
    int length = (int)source.length();  // Convert length to signed int. Required to allow negative values.

    // Split ngrams
    for (int n = minn; n <= maxn; n++) {
      if (NgramItself::NEVER == itself && length == n)
        continue;

      for (int pos = 0; pos <= length - n; pos++) {
        u32string ngram = u32string(source, pos, n);

        target.push_back(ngram);
      }
    }

    if (NgramItself::ALWAYS == itself && (length < minn || length > maxn)) {
      target.push_back(source);
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("ExpandCharNgrams").Device(DEVICE_CPU), ExpandCharNgramsOp);
