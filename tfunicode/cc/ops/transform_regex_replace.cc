#include "tensorflow/core/framework/op.h"
#include "tfunicode/cc/lib/transform_shape.h"


REGISTER_OP("TransformRegexReplace")
  .Input("source: string")
  .Attr("pattern: list(string) >= 1")
  .Attr("rewrite: list(string) >= 1")
  .Output("result: string")
  .SetShapeFn(TransformBaseShape);
//  .SetIsStateful();
