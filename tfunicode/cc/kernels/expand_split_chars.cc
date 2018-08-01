#include "tfunicode/cc/lib/expand_base.h"

using namespace std;


class ExpandSplitCharsOp : public ExpandBaseOp {
 public:
  explicit ExpandSplitCharsOp(OpKernelConstruction* ctx) : ExpandBaseOp(ctx) {}

 private:
  inline void expand(const u32string &source, std::vector<u32string> &target) {
    if (source.length() == 0) {
      return;
    }

    if (source.length() == 1) {
      target.push_back(source);
      return;
    }

    // Split chars
    for (int32_t pos = 0; pos < source.length(); pos++) {
      u32string word = u32string(source, pos, 1);

      target.push_back(word);
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("ExpandSplitChars").Device(DEVICE_CPU), ExpandSplitCharsOp);
