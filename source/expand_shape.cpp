#ifndef TF_UC_OPS_EXPAND_SHAPE
#define TF_UC_OPS_EXPAND_SHAPE

#include "tensorflow/core/framework/shape_inference.h"

using namespace tensorflow;


Status ExpandBaseShape(shape_inference::InferenceContext* c) {
    const uint32 source_rank = c->Rank(c->input(0));

    c->set_output(0, c->Matrix(shape_inference::InferenceContext::kUnknownDim, source_rank + 1)); // indices
    c->set_output(1, c->Vector(shape_inference::InferenceContext::kUnknownDim)); // values
    c->set_output(2, c->Vector(source_rank + 1)); // shape

    return Status::OK();
}


#endif
