# tfunicode

Infrastructure to build TensorFlow custom ops wheel for unicode string preprocessing.
For more info about provided ops see package [README](https://github.com/shkarupa-alex/tfunicode/blob/master/pip_package/README.md)

## Develop commands

```bash
bazel build //tfunicode
bazel test //tfunicode/...
```

## Build commands

```bash
bazel clean --expunge
bazel build //pip_package
bazel-bin/pip_package/pip_package ./wheels
```

To build Python3-package run before normal build:

```bash
export PYTHON_BIN_PATH=python3
```
