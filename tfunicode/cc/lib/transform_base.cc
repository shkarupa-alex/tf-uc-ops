#include "tfunicode/cc/lib/transform_base.h"
#include "unilib/utf8.h"

using namespace tensorflow;
using namespace ufal::unilib;
using namespace std;


void TransformBaseOp::Compute(OpKernelContext* ctx) {
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

    utf8::decode(binary_string, unicode_string);
    transform(unicode_string);
    utf8::encode(unicode_string, binary_string);

    result_values(i) = binary_string;
  }
}
