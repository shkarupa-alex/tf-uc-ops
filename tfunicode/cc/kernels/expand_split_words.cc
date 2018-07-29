#include "tfunicode/cc/lib/expand_base.h"

using namespace std;


class ExpandSplitWordsOp : public ExpandBaseOp {
 public:
  explicit ExpandSplitWordsOp(OpKernelConstruction* ctx) : ExpandBaseOp(ctx) {}

 private:
  void expand(const u32string &source, std::vector<u32string> &target) {
    if (source.length() < 2) {
      target.push_back(source);
      return;
    }

    // Split words by Unicode rules
//    BreakIterator *wordIterator = _wordIterator->clone();
//    wordIterator->setText(source);
//    int32_t prev = wordIterator->first();
//    for (int32_t pos = wordIterator->first(); pos != BreakIterator::DONE; pos = wordIterator->next()) {
//      if (prev == pos) {
//        continue;
//      }
//
//      UnicodeString word = UnicodeString(source, prev, pos - prev);
//      target.push_back(word);
//
//      prev = pos;
//    }
  }
};

REGISTER_KERNEL_BUILDER(Name("ExpandSplitWords").Device(DEVICE_CPU), ExpandSplitWordsOp);
