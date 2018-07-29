#include "tfunicode/cc/lib/transform_base.h"
#include "unilib/uninorms.h"

using namespace ufal::unilib;
using namespace std;


class TransformNormalizeUnicodeOp : public TransformBaseOp {
 public:
  explicit TransformNormalizeUnicodeOp(OpKernelConstruction* ctx) : TransformBaseOp(ctx) {
    // Prepare attr
    OP_REQUIRES_OK(ctx, ctx->GetAttr("form", &form_value));
    std::transform(form_value.begin(), form_value.end(), form_value.begin(), ::toupper);
  }

 private:
    string form_value;

 protected:
  void transform(u32string &item) {
    if("NFC" == form_value) {
      uninorms::nfc(item);
    } else if ("NFD" == form_value) {
      uninorms::nfd(item);
    } else if ("NFKC" == form_value) {
      uninorms::nfkc(item);
    } else if ("NFKD" == form_value) {
      uninorms::nfkd(item);
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("TransformNormalizeUnicode").Device(DEVICE_CPU), TransformNormalizeUnicodeOp);
