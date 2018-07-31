#pragma once

#include "tensorflow/core/framework/shape_inference.h"

using namespace tensorflow;


Status TransformBaseShape(shape_inference::InferenceContext* c);
