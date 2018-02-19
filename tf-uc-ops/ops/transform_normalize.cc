#ifndef TF_UC_OPS_NORMALIZE_UNICODE
#define TF_UC_OPS_NORMALIZE_UNICODE

#include "transform_base.cc"
#include <unicode/normalizer2.h>

REGISTER_OP("NormalizeUnicode")
  .Input("source: string")
  .Attr("form: string")
  .Output("result: string")
  .SetShapeFn([](shape_inference::InferenceContext* c) {
    c->set_output(0, c->input(0));

    return Status::OK();
  })
  .SetIsStateful();


class NormalizeUnicodeOp : public TransformBaseOp {
 public:
  explicit NormalizeUnicodeOp(OpKernelConstruction* ctx) : TransformBaseOp(ctx) {
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
    OP_REQUIRES(ctx, U_SUCCESS(instanceError), errors::InvalidArgument("Normalizer2 instantiation failed"));
  }

 private:
    mutex normalizerMutex;
    const Normalizer2 *normalizerInstance GUARDED_BY(normalizerMutex);

 protected:
  void transform(UnicodeString &item, UErrorCode &error) {
    item = normalizerInstance->normalize(item, error);
  }
};

REGISTER_KERNEL_BUILDER(Name("NormalizeUnicode").Device(DEVICE_CPU), NormalizeUnicodeOp);

#endif
