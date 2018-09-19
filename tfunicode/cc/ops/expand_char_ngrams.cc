#include "tensorflow/core/framework/op.h"
#include "tfunicode/cc/lib/expand_shape.h"


REGISTER_OP("ExpandCharNgrams")
  .Input("source: string")
  .Attr("minn: int")
  .Attr("maxn: int")
  .Attr("itself: {'ASIS', 'NEVER', 'ALWAYS', 'ALONE'}")
  .Output("indices: int64")
  .Output("values: string")
  .Output("shape: int64")
  .SetShapeFn(ExpandBaseShape)
  .SetIsStateful();
