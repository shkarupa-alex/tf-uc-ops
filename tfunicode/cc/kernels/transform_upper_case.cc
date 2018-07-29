#include "tfunicode/cc/lib/transform_base.h"
#include "unilib/unicode.h"

using namespace ufal::unilib;
using namespace std;


class TransformUpperCaseOp : public TransformBaseOp {
 public:
  explicit TransformUpperCaseOp(OpKernelConstruction* ctx) : TransformBaseOp(ctx) {}

 protected:
  void transform(u32string &item) {
    for(char32_t& c : item) {
      c = unicode::uppercase(c);
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("TransformUpperCase").Device(DEVICE_CPU), TransformUpperCaseOp);
