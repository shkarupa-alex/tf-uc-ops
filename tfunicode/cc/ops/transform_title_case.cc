#include "tensorflow/core/framework/op.h"
#include "tfunicode/cc/lib/transform_shape.h"


REGISTER_OP("TransformTitleCase")
  .Input("source: string")
  .Output("result: string")
  .SetShapeFn(TransformBaseShape)
  .SetIsStateful();
