#ifndef TF_UC_OPS_TRANSFORM_SHAPE
#define TF_UC_OPS_TRANSFORM_SHAPE

#include "tensorflow/core/framework/shape_inference.h"

using namespace tensorflow;


Status TransformBaseShape(shape_inference::InferenceContext* c) {
  c->set_output(0, c->input(0));
  return Status::OK();
}

#endif
