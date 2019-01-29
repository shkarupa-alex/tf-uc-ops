#include "tensorflow/core/framework/op.h"
#include "tfunicode/cc/lib/transform_shape.h"


REGISTER_OP("TransformStringReplace")
  .Input("source: string")
  .Attr("needle: list(string) >= 1")
  .Attr("haystack: list(string) >= 1")
  .Output("result: string")
  .SetShapeFn(TransformBaseShape);
//  .SetIsStateful();
