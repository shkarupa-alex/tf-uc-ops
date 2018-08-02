#include "tfunicode/cc/lib/transform_base.h"
#include "unilib/unicode.h"

using namespace ufal::unilib;
using namespace std;


class TransformTitleCaseOp : public TransformBaseOp {
 public:
  explicit TransformTitleCaseOp(OpKernelConstruction* ctx) : TransformBaseOp(ctx) {}

 protected:
  inline void transform(u32string &item) {
    for(char32_t& c : item) {
      c = unicode::lowercase(c);
    }

    if (0 != item.length()) {
      const char32_t first = item[0];
      char32_t title = unicode::titlecase(first);

      if (first == title) {
        title = unicode::uppercase(first);
      }

      item[0] = title;
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("TransformTitleCase").Device(DEVICE_CPU), TransformTitleCaseOp);
