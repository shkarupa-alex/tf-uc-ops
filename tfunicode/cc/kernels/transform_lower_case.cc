#include "tfunicode/cc/lib/transform_base.h"
#include "unilib/unicode.h"

using namespace ufal::unilib;
using namespace std;


class TransformLowerCaseOp : public TransformBaseOp {
 public:
  explicit TransformLowerCaseOp(OpKernelConstruction* ctx) : TransformBaseOp(ctx) {}

 protected:
  inline void transform(u32string &item) {
    for(char32_t& c : item) {
      c = unicode::lowercase(c);
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("TransformLowerCase").Device(DEVICE_CPU), TransformLowerCaseOp);
