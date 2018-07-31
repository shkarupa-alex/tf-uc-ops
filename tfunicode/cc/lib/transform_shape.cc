#include "tfunicode/cc/lib/transform_shape.h"

using namespace tensorflow;


Status TransformBaseShape(shape_inference::InferenceContext* c) {
  c->set_output(0, c->input(0));

  return Status::OK();
}
