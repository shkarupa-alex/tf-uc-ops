#ifndef TF_UC_OPS_SPLIT_WORDS
#define TF_UC_OPS_SPLIT_WORDS

#include "expand_base.cc"
#include <unicode/brkiter.h>

REGISTER_OP("SplitWords")
  .Input("source: string")
  .Attr("default_value: string")
  .Output("result: string")
  .SetShapeFn([](shape_inference::InferenceContext* c) {
    shape_inference::ShapeHandle input = c->input(0);
    shape_inference::ShapeHandle append = c->Vector(shape_inference::InferenceContext::kUnknownDim);

    shape_inference::ShapeHandle output;
    TF_RETURN_IF_ERROR(c->Concatenate(input, append, &output));

    c->set_output(0, output);

    return Status::OK();
  })
  .SetIsStateful();


class SplitWordsOp : public ExpandBaseOp {
 public:
  explicit SplitWordsOp(OpKernelConstruction* ctx) : ExpandBaseOp(ctx) {
    // Create word-level BreakIterator instance
    UErrorCode wordError = U_ZERO_ERROR;
    wordIterator = BreakIterator::createWordInstance(Locale::getRoot(), wordError);
    OP_REQUIRES(ctx, U_SUCCESS(wordError), errors::InvalidArgument("BreakIterator instantiation failed"));
  }

 private:
  mutex wordMutex;
  BreakIterator *wordIterator GUARDED_BY(wordMutex);

  void expand(const UnicodeString &source, std::vector<UnicodeString> &target, UErrorCode &error) {
    if (0 == source.length()) {
      target.push_back(UnicodeString());
      return;
    }

    // Split words by Unicode rules
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

REGISTER_KERNEL_BUILDER(Name("SplitWords").Device(DEVICE_CPU), SplitWordsOp);

#endif
