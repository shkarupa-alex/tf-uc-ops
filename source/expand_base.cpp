#ifndef TF_UC_OPS_EXPAND_BASE
#define TF_UC_OPS_EXPAND_BASE

#include <unicode/unistr.h>
#include "tensorflow/core/framework/op_kernel.h"
#include "expand_shape.cpp"

using icu::UnicodeString;


class ExpandBaseOp : public OpKernel {
 public:
  explicit ExpandBaseOp(OpKernelConstruction* ctx) : OpKernel(ctx) {}

  void Compute(OpKernelContext* ctx) override {
    // Prepare source
    const Tensor* source_tensor;
    OP_REQUIRES_OK(ctx, ctx->input("source", &source_tensor));
    const auto source_values = source_tensor->flat<string>();

    const uint64 source_len = source_tensor->shape().num_elements();
    const uint64 source_rank = source_tensor->shape().dims();
    const uint64 result_rank = source_rank + 1;


    // Prepare intermediate result storage
    uint64 expand_max = 0;

    std::vector<string> intermediate_values;
    intermediate_values.reserve(source_len * expand_rate);

    std::vector<uint64> intermediate_indices;
    intermediate_indices.reserve(source_len * expand_rate);

    std::vector<UnicodeString> expanded_storage;
    expanded_storage.reserve(expand_rate);


    // Expand source values
    for (uint64 v = 0; v < source_len; v++) {
      string binary_string = source_values(v);
      UnicodeString unicode_string = UnicodeString::fromUTF8(binary_string);

      UErrorCode expandError = U_ZERO_ERROR;
      expanded_storage.clear();
      expand(unicode_string, expanded_storage, expandError);
      OP_REQUIRES(ctx, U_SUCCESS(expandError), errors::InvalidArgument("unicode expanding failed"));

      expand_max = expanded_storage.size() > expand_max ? expanded_storage.size() : expand_max;

      for (uint64 e = 0; e < expanded_storage.size(); e++) {
        binary_string.clear();
        expanded_storage[e].toUTF8String(binary_string);

        intermediate_values.push_back(binary_string);
        intermediate_indices.push_back(v);
      }
    }


    // Allocate result
    Tensor* result_indices_tensor;
    OP_REQUIRES_OK(
      ctx,
      ctx->allocate_output(
        0,
        TensorShape({(int64)intermediate_values.size(), (int64)result_rank}),
        &result_indices_tensor
      )
    );
    auto result_indices = result_indices_tensor->flat<int64>();

    Tensor* result_values_tensor;
    OP_REQUIRES_OK(
      ctx,
      ctx->allocate_output(
        1,
        TensorShape({(int64)intermediate_values.size()}),
        &result_values_tensor
      )
    );
    auto result_values = result_values_tensor->flat<string>();

    Tensor* result_shape_tensor;
    OP_REQUIRES_OK(
      ctx,
      ctx->allocate_output(
        2,
        TensorShape({(int64)result_rank}),
        &result_shape_tensor
      )
    );
    auto result_shape = result_shape_tensor->flat<int64>();


    // Fill result
    for (uint64 v = 0; v < intermediate_values.size(); v++) {
      result_values(v) = intermediate_values[v];
    }

    uint64 last_source_index = -1;
    uint64 new_dim_index = -1;

    std::vector<uint64> last_source_coords;
    last_source_coords.reserve(source_rank);

    std::vector<uint64> source_index_divisor;
    source_index_divisor.reserve(source_rank);
    for (uint64 d = 1; d < source_rank + 1; d++) {
      uint64 divisor = 1;
      for (uint64 p = d; p < source_rank; p++) {
        divisor *= source_tensor->shape().dim_size(p);
      }
      source_index_divisor.push_back(divisor);
    }

    for (uint64 i = 0; i < intermediate_indices.size(); i++) {
      if (intermediate_indices[i] != last_source_index) {
        new_dim_index = 0;
        last_source_index = intermediate_indices[i];

        last_source_coords.clear();
        uint64 curr_source_index = last_source_index;
        for (uint64 c = 0; c < source_index_divisor.size(); c++) {
          uint64 curr_index_value = curr_source_index / source_index_divisor[c];
          last_source_coords.push_back(curr_index_value);
          curr_source_index -= curr_index_value * source_index_divisor[c];
        }
      }

      for (uint64 c = 0; c < source_rank; c++) {
        result_indices(i * result_rank + c) = last_source_coords[c];
      }
      result_indices(i * result_rank + source_rank) = new_dim_index;

      new_dim_index++;
    }

    for (uint64 s = 0; s < source_rank; s++) {
      result_shape(s) = source_tensor->shape().dim_size(s);
    }
    result_shape(source_rank) = expand_max;
  }
 protected:
  const uint64 expand_rate = 5;
  virtual void expand(const UnicodeString &source, std::vector<UnicodeString> &target, UErrorCode &error) = 0;
};

#endif
