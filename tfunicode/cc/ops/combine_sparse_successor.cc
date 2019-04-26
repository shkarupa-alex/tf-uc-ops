#include "tensorflow/core/framework/op.h"
#include "tensorflow/core/framework/shape_inference.h"

using namespace tensorflow;


REGISTER_OP("CombineSparseSuccessor")
  .Input("parent_indices: int64")
  .Input("parent_shape: int64")
  .Input("child_indices: int64")
  .Input("child_values: string")
  .Input("child_shape: int64")
  .Output("result_indices: int64")
  .Output("result_values: string")
  .Output("result_shape: int64")
  .SetShapeFn([](shape_inference::InferenceContext* c) {
    shape_inference::ShapeHandle unused;
    TF_RETURN_IF_ERROR(c->WithRank(c->input(0), 2, &unused));
    TF_RETURN_IF_ERROR(c->WithRank(c->input(1), 1, &unused));
    TF_RETURN_IF_ERROR(c->WithRank(c->input(2), 2, &unused));
    TF_RETURN_IF_ERROR(c->WithRank(c->input(3), 1, &unused));
    TF_RETURN_IF_ERROR(c->WithRank(c->input(4), 1, &unused));

    const uint64 parent_indices_1 = c->Value(c->Dim(c->input(0), 1));
    const uint64 child_indices_0 = c->Value(c->Dim(c->input(2), 0));
    c->set_output(0, c->Matrix(
      child_indices_0,
      parent_indices_1 + 1
    ));
    c->set_output(1, c->input(3));
    c->set_output(2, c->Vector(c->Value(c->NumElements(c->input(1))) + 1));

    return Status::OK();
  });
//  .SetIsStateful();
