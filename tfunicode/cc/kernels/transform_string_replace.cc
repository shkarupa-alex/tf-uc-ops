#include "tfunicode/cc/lib/transform_base.h"
#include "unilib/utf8.h"

using namespace ufal::unilib;
using namespace std;


class TransformStringReplaceOp : public TransformBaseOp {
 public:
  explicit TransformStringReplaceOp(OpKernelConstruction* ctx) : TransformBaseOp(ctx) {
    // Prepare attrs
    std::vector<string> _needle;
    OP_REQUIRES_OK(ctx, ctx->GetAttr("needle", &_needle));
    needle.resize(_needle.size());
    for (uint64 i = 0; i < _needle.size(); i++) {
      utf8::decode(_needle[i], needle[i]);
      OP_REQUIRES(ctx, needle[i].size() > 0, errors::InvalidArgument("Items of \"needle\" could not be empty"));
    }

    std::vector<string> _haystack;
    OP_REQUIRES_OK(ctx, ctx->GetAttr("haystack", &_haystack));
    haystack.resize(_haystack.size());
    for (uint64 i = 0; i < _haystack.size(); i++) {
      utf8::decode(_haystack[i], haystack[i]);
    }

    OP_REQUIRES(ctx, needle.size() == haystack.size(), errors::InvalidArgument("Sizes are different for \"needle\" and \"haystack\""));
  }

 private:
  std::vector<u32string> needle;
  std::vector<u32string> haystack;

 protected:
  inline void transform(u32string &item) {
    for (uint64 i = 0; i < needle.size(); i++) {
      uint64 pos = item.find(needle[i]);

      while(pos != std::u32string::npos)
      {
        item.replace(pos, needle[i].size(), haystack[i]);
        pos = item.find(needle[i], pos + haystack[i].size());
      }
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("TransformStringReplace").Device(DEVICE_CPU), TransformStringReplaceOp);
