#include "tensorflow/core/framework/op_kernel.h"
#include "re2/re2.h"
#include "unilib/utf8.h"
#include "tensorflow/core/util/ptr_util.h"

using namespace tensorflow;
using namespace ufal::unilib;
using namespace std;

// This OP could not be easily inherited from TransformBaseOp due to RE2 woks with binary strings
class TransformRegexReplaceOp : public OpKernel {
 public:
  explicit TransformRegexReplaceOp(OpKernelConstruction* ctx) : OpKernel(ctx) {
    // Prepare attrs
    std::vector<string> _pattern;
    OP_REQUIRES_OK(ctx, ctx->GetAttr("pattern", &_pattern));
    pattern.resize(_pattern.size());
    for (uint64 i = 0; i < _pattern.size(); i++) {
      OP_REQUIRES(ctx, _pattern[i].size() > 0, errors::InvalidArgument("Items of \"pattern\" could not be empty"));

      pattern[i] = MakeUnique<RE2>(_pattern[i]);
      OP_REQUIRES(ctx, pattern[i]->ok(), errors::InvalidArgument("Invalid pattern: ", _pattern[i], ", error: ", pattern[i]->error()));
    }

    std::vector<string> _rewrite;
    OP_REQUIRES_OK(ctx, ctx->GetAttr("rewrite", &rewrite));

    OP_REQUIRES(ctx, pattern.size() == rewrite.size(), errors::InvalidArgument("Sizes are different for \"pattern\" and \"rewrite\""));
  }

  void Compute(OpKernelContext* ctx) {
    // Prepare source
    const Tensor* source_tensor;
    OP_REQUIRES_OK(ctx, ctx->input("source", &source_tensor));
    const auto source_values = source_tensor->flat<string>();
    const uint64 num_elements = source_tensor->shape().num_elements();


    // Allocate result
    Tensor* result_tensor;
    OP_REQUIRES_OK(ctx, ctx->allocate_output(0, TensorShape({source_tensor->shape()}), &result_tensor));
    auto result_values = result_tensor->flat<string>();


    // Transform and write to output
    for (uint64 i = 0; i < num_elements; i++) {
      string binary_string = source_values(i);
      u32string unicode_string;

      bool validate_result = utf8::valid(binary_string);
      OP_REQUIRES(ctx, validate_result, errors::InvalidArgument("invalid utf-8 source string"));

      transform(binary_string);

      result_values(i) = binary_string;
    }
  }

 private:
  std::vector<std::unique_ptr<RE2>> pattern;
  std::vector<string> rewrite;

 protected:
  inline void transform(string &item) {
    for (uint64 i = 0; i < pattern.size(); i++) {
      RE2::GlobalReplace(&item, *pattern[i], rewrite[i]);
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("TransformRegexReplace").Device(DEVICE_CPU), TransformRegexReplaceOp);
