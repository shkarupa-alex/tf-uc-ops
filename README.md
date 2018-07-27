# tfunicode

Demo repository with infrastructure to build wheel for [Tensorflow custom op](https://www.tensorflow.org/extend/adding_an_op)

## Develop commands

```bash
bazel build //tfunicode
bazel test //tfunicode/...
```

## Build commands

```bash
bazel clean --expunge
bazel build //pip_package
bazel-bin/pip_package/pip_package ~/wheels
```

To build with Python3 support:

```bash
export PYTHON_BIN=python3
bazel-bin/pip_package/pip_package ~/wheels
```
