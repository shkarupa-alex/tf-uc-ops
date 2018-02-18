#ifndef TF_UC_OPS_TRANSFORM_BASE
#define TF_UC_OPS_TRANSFORM_BASE

#include "tensorflow/core/framework/op.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/shape_inference.h"
#include <unicode/unistr.h>

using namespace tensorflow;

class TransformBaseOp : public OpKernel {
 public:
  explicit TransformBaseOp(OpKernelConstruction* ctx) : OpKernel(ctx) {}

  void Compute(OpKernelContext* ctx) override {
    // Prepare source
    const Tensor* source_tensor;
    OP_REQUIRES_OK(ctx, ctx->input("source", &source_tensor));
    const auto source_values = source_tensor->flat<string>();
    const int64 num_elements = source_tensor->shape().num_elements();


    // Allocate result
    Tensor* result_tensor;
    OP_REQUIRES_OK(ctx, ctx->allocate_output(0, TensorShape({source_tensor->shape()}), &result_tensor));
    auto result_values = result_tensor->flat<string>();


    // Transform and write to output
    for (int64 i = 0; i < num_elements; i++) {
      string binary_string = source_values(i);
      UnicodeString unicode_string = UnicodeString::fromUTF8(binary_string);

      UErrorCode transformError = U_ZERO_ERROR;
      transform(unicode_string, transformError);
      OP_REQUIRES(ctx, U_SUCCESS(transformError), errors::InvalidArgument("unicode transformation failed"));

      binary_string.clear();
      unicode_string.toUTF8String(binary_string);
      result_values(i) = binary_string;
    }
  }
 protected:
  virtual void transform(UnicodeString &target, UErrorCode &error) = 0;
};

#endif
