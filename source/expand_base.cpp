#ifndef TF_UC_OPS_EXPAND_BASE
#define TF_UC_OPS_EXPAND_BASE

#include "tensorflow/core/framework/op.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/shape_inference.h"
#include <unicode/unistr.h>
#include <unicode/bytestream.h>

using namespace tensorflow;

class ExpandBaseOp : public OpKernel {
 public:
  explicit ExpandBaseOp(OpKernelConstruction* ctx) : OpKernel(ctx) {
      // Prepare attr
    OP_REQUIRES_OK(ctx, ctx->GetAttr("default_value", &default_value));
  }

  void Compute(OpKernelContext* ctx) override {
    // Prepare source
    const Tensor* source_tensor;
    OP_REQUIRES_OK(ctx, ctx->input("source", &source_tensor));
    const auto source_values = source_tensor->flat<string>();
    const uint64 num_elements = source_tensor->shape().num_elements();


    std::vector<std::vector<string>> result_strings;
    result_strings.reserve(num_elements);
    uint64 max_size = 0;

    // Expand and store in temporary array (better speed, worse memory consumption)
    for (uint64 i = 0; i < num_elements; i++) {
      string binary_string = source_values(i);
      UnicodeString unicode_string = UnicodeString::fromUTF8(binary_string);

      std::vector<UnicodeString> temp_unicodes;
      temp_unicodes.clear();

      UErrorCode expandError = U_ZERO_ERROR;
      expand(unicode_string, temp_unicodes, expandError);
      OP_REQUIRES(ctx, U_SUCCESS(expandError), errors::InvalidArgument("unicode expanding failed"));

      std::vector<string> temp_strings;
      temp_strings.clear();

      for(UnicodeString t : temp_unicodes) {
        binary_string.clear();
        t.toUTF8String(binary_string);
        temp_strings.push_back(binary_string);
      }
      max_size = temp_strings.size() > max_size ? temp_strings.size() : max_size;
      result_strings.push_back(temp_strings);
    }


    // Allocate and write result
    TensorShape result_shape(source_tensor->shape());
    result_shape.AddDim(max_size);

    Tensor* result_tensor;
    OP_REQUIRES_OK(ctx, ctx->allocate_output(0, TensorShape({result_shape}), &result_tensor));
    auto result_values = result_tensor->flat<string>();

    for (uint64 i = 0; i < result_strings.size(); i++) {
      for (uint64 j = 0; j < result_strings[i].size(); j++) {
        result_values(i * max_size + j) = result_strings[i][j];
      }
      for (uint64 k = 0; k < max_size - result_strings[i].size(); k++) {
        result_values(i * max_size + result_strings[i].size() + k) = default_value;
      }
    }
  }
 private:
  string default_value;

 protected:
  virtual void expand(const UnicodeString &source, std::vector<UnicodeString> &target, UErrorCode &error) = 0;
};

#endif
