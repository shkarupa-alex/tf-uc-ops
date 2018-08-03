# tfunicode

Infrastructure to build TensorFlow custom ops wheel for unicode string preprocessing.
For more info about provided ops see package [README](https://github.com/shkarupa-alex/tfunicode/blob/master/pip_package/README.md)

## Develop commands

```bash
bazel build //tfunicode
bazel test //tfunicode/...
```

## Build release within local MacOS X

```bash
bazel clean --expunge

export PYTHON_BIN_PATH=python2.7
bazel build --copt="-D_GLIBCXX_USE_CXX11_ABI=0" //pip_package
bazel-bin/pip_package/pip_package ./wheels

export PYTHON_BIN_PATH=python3.6
bazel build --copt="-D_GLIBCXX_USE_CXX11_ABI=0" //pip_package
bazel-bin/pip_package/pip_package ./wheels
```

## Build release within local MacOS X

```bash
docker run -it -v `pwd`:/tfunicode tensorflow/tensorflow:1.8.0-devel /tfunicode/pip_package/build_linux_release.sh
```

