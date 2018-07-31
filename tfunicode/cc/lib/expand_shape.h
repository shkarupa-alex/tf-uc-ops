#pragma once

#include "tensorflow/core/framework/shape_inference.h"

using namespace tensorflow;


Status ExpandBaseShape(shape_inference::InferenceContext* c);
