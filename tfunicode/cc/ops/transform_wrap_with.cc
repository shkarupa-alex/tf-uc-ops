#include "tensorflow/core/framework/op.h"
#include "tfunicode/cc/lib/transform_shape.h"


REGISTER_OP("TransformWrapWith")
  .Input("source: string")
  .Attr("left: string")
  .Attr("right: string")
  .Output("result: string")
  .SetShapeFn(TransformBaseShape)
  .SetIsStateful();
