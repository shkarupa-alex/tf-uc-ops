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
export PYTHON_BIN_PATH=python3
bazel clean --expunge
bazel build --cxxopt="-D_GLIBCXX_USE_CXX11_ABI=0" //pip_package
bazel-bin/pip_package/pip_package ./wheels
```
