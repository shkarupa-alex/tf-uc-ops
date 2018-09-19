#include "tensorflow/core/framework/op.h"
#include "tfunicode/cc/lib/transform_shape.h"


REGISTER_OP("TransformStringReplace")
  .Input("source: string")
  .Attr("old: list(string)")
  .Attr("new: list(string)")
  .Output("result: string")
  .SetShapeFn(TransformBaseShape)
  .SetIsStateful();
