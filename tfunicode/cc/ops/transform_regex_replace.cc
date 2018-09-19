#include "tensorflow/core/framework/op.h"
#include "tfunicode/cc/lib/transform_shape.h"


REGISTER_OP("TransformRegexReplace")
  .Input("source: string")
  .Attr("pattern: list(string)")
  .Attr("rewrite: list(string)")
  .Attr("global: bool = true")
  .Output("result: string")
  .SetShapeFn(TransformBaseShape)
  .SetIsStateful();
