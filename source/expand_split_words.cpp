#ifndef TF_UC_OPS_EXPAND_SPLIT_WORDS
#define TF_UC_OPS_EXPAND_SPLIT_WORDS

#include <unicode/unistr.h>
#include <unicode/brkiter.h>
#include "expand_base.cpp"


REGISTER_OP("ExpandSplitWords")
  .Input("source: string")
  .Output("indices: int64")
  .Output("values: string")
  .Output("shape: int64")
  .SetShapeFn(ExpandBaseShape)
  .SetIsStateful();


class ExpandSplitWordsOp : public ExpandBaseOp {
 public:
  explicit ExpandSplitWordsOp(OpKernelConstruction* ctx) : ExpandBaseOp(ctx) {
    // Create word-level BreakIterator instance
    UErrorCode wordError = U_ZERO_ERROR;
    _wordIterator = BreakIterator::createWordInstance(Locale::getRoot(), wordError);
    OP_REQUIRES(ctx, U_SUCCESS(wordError), errors::InvalidArgument("BreakIterator instantiation failed"));
  }

 private:
  mutex wordMutex;
  BreakIterator *_wordIterator GUARDED_BY(wordMutex);

  void expand(const UnicodeString &source, std::vector<UnicodeString> &target, UErrorCode &error) {
    if (source.length() < 2) {
      target.push_back(source);
      return;
    }

    // Split words by Unicode rules
    BreakIterator *wordIterator = _wordIterator->clone();
    wordIterator->setText(source);
    int32_t prev = wordIterator->first();
    for (int32_t pos = wordIterator->first(); pos != BreakIterator::DONE; pos = wordIterator->next()) {
      if (prev == pos) {
        continue;
      }

      UnicodeString word = UnicodeString(source, prev, pos - prev);
      target.push_back(word);

      prev = pos;
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("ExpandSplitWords").Device(DEVICE_CPU), ExpandSplitWordsOp);

#endif
