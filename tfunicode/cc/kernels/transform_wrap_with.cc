#include "tfunicode/cc/lib/transform_base.h"
#include "unilib/utf8.h"

using namespace ufal::unilib;
using namespace std;


class TransformWrapWithOp : public TransformBaseOp {
 public:
  explicit TransformWrapWithOp(OpKernelConstruction* ctx) : TransformBaseOp(ctx) {
    // Prepare attrs
    string _left;
    OP_REQUIRES_OK(ctx, ctx->GetAttr("left", &_left));
    utf8::decode(_left, left);

    string _right;
    OP_REQUIRES_OK(ctx, ctx->GetAttr("right", &_right));
    utf8::decode(_right, right);
  }

 private:
  u32string left;
  u32string right;

 protected:
  inline void transform(u32string &item) {
    item = left + item + right;
  }
};

REGISTER_KERNEL_BUILDER(Name("TransformWrapWith").Device(DEVICE_CPU), TransformWrapWithOp);
