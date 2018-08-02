#include "tensorflow/core/framework/op.h"
#include "tfunicode/cc/lib/expand_shape.h"


REGISTER_OP("ExpandSplitWords")
  .Input("source: string")
  .Output("indices: int64")
  .Output("values: string")
  .Output("shape: int64")
  .SetShapeFn(ExpandBaseShape)
  .SetIsStateful();
