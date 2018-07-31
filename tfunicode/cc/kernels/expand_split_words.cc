#include "tfunicode/cc/lib/expand_base.h"
#include "tfunicode/cc/lib/word_break.h"

using namespace std;


class ExpandSplitWordsOp : public ExpandBaseOp {
 public:
  explicit ExpandSplitWordsOp(OpKernelConstruction* ctx) : ExpandBaseOp(ctx) {}

 private:
  static const set<char32_t> extended_pictographic;

  void expand(const u32string &source, std::vector<u32string> &target) {
    if (source.length() < 2) {
      target.push_back(source);
      return;
    }

    int prev = 0;
    for (int pos = 1; pos <= (int)source.length(); pos++) {
      if (!WordBreak::IsBreak(source, pos)) {
        continue;
      }

      u32string word = u32string(source, prev, pos - prev);
      target.push_back(word);
      prev = pos;
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("ExpandSplitWords").Device(DEVICE_CPU), ExpandSplitWordsOp);
