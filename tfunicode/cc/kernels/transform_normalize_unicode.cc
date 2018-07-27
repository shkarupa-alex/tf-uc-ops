#include "tensorflow/core/framework/op.h"
#include "tfunicode/cc/lib/transform_base.h"
#include <unicode/unistr.h>
#include <unicode/normalizer2.h>

using icu::Normalizer2;
using icu::UnicodeString;


class TransformNormalizeUnicodeOp : public TransformBaseOp {
 public:
  explicit TransformNormalizeUnicodeOp(OpKernelConstruction* ctx) : TransformBaseOp(ctx) {
    // Prepare attr
    string form_value;
    OP_REQUIRES_OK(ctx, ctx->GetAttr("form", &form_value));
    std::transform(form_value.begin(), form_value.end(), form_value.begin(), ::toupper);


    // Create Normalizer2 instance
    UErrorCode instanceError = U_ZERO_ERROR;
    if("NFC" == form_value) {
      normalizerInstance = Normalizer2::getNFCInstance(instanceError);
    } else if ("NFD" == form_value) {
      normalizerInstance = Normalizer2::getNFDInstance(instanceError);
    } else if ("NFKC" == form_value) {
      normalizerInstance = Normalizer2::getNFKCInstance(instanceError);
    } else if ("NFKD" == form_value) {
      normalizerInstance = Normalizer2::getNFKDInstance(instanceError);
    } else {
      OP_REQUIRES(ctx, false, errors::InvalidArgument("unknown normalization form"));
    }
    OP_REQUIRES(ctx, U_SUCCESS(instanceError), errors::Internal("Normalizer2 instantiation failed"));
  }

 private:
    mutex normalizerMutex;
    const Normalizer2 *normalizerInstance GUARDED_BY(normalizerMutex);

 protected:
  void transform(UnicodeString &item, UErrorCode &error) {
    item = normalizerInstance->normalize(item, error);
  }
};

REGISTER_KERNEL_BUILDER(Name("TransformNormalizeUnicode").Device(DEVICE_CPU), TransformNormalizeUnicodeOp);
