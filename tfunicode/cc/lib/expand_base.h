#pragma once

#include "tensorflow/core/framework/op_kernel.h"

using namespace tensorflow;
using namespace std;


class ExpandBaseOp : public OpKernel {
 public:
  explicit ExpandBaseOp(OpKernelConstruction* ctx) : OpKernel(ctx) {}

  void Compute(OpKernelContext* ctx);

 protected:
  const uint64 expand_rate = 5;

  virtual void expand(const u32string &source, std::vector<u32string> &target) = 0;
};
