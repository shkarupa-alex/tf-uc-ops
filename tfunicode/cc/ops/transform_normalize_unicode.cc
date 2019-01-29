#include "tensorflow/core/framework/op.h"
#include "tfunicode/cc/lib/transform_shape.h"


REGISTER_OP("TransformNormalizeUnicode")
  .Input("source: string")
  .Attr("form: {'NFC', 'NFD', 'NFKC', 'NFKD'}")
  .Output("result: string")
  .SetShapeFn(TransformBaseShape);
//  .SetIsStateful();
