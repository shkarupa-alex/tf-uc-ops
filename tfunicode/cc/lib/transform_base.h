#pragma once

#include "tensorflow/core/framework/op_kernel.h"

using namespace tensorflow;
using namespace std;


class TransformBaseOp : public OpKernel {
 public:
  explicit TransformBaseOp(OpKernelConstruction* ctx) : OpKernel(ctx) {};

  void Compute(OpKernelContext* ctx);
 protected:
  virtual void transform(u32string &item) = 0;
};
