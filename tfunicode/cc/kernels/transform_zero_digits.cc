#include "tfunicode/cc/lib/transform_base.h"
#include "unilib/unicode.h"

using namespace ufal::unilib;
using namespace std;


class TransformZeroDigitsOp : public TransformBaseOp {
 public:
  explicit TransformZeroDigitsOp(OpKernelConstruction* ctx) : TransformBaseOp(ctx) {}

 private:
  const char32_t zero = 48; // Unicode 0

 protected:
  inline void transform(u32string &item) {
    for(char32_t& c : item) {
      if (unicode::category(c) & unicode::N) {
        c = zero;
      }
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("TransformZeroDigits").Device(DEVICE_CPU), TransformZeroDigitsOp);
