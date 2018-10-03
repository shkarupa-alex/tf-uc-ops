# tfunicode

Infrastructure to build TensorFlow custom ops wheel for unicode string preprocessing.
For more info about provided ops see package [README](https://github.com/shkarupa-alex/tfunicode/blob/master/pip_package/README.md)

## Develop environment
1. Install all [dependencies including python headers](https://www.tensorflow.org/install/install_sources).
2. Update python paths in tools/bazel.rc (one version for all builds).

P.S.
Do not use pyenv, otherwise tests mostly likely will fail.

## Develop commands

```bash
bazel clean --expunge
bazel build //tfunicode
bazel test --test_output=errors //tfunicode/...
```

## Build release within local MacOS X

```bash
export PYTHON_BIN_PATH=python2.7
bazel build --copt="-D_GLIBCXX_USE_CXX11_ABI=0" //pip_package
bazel-bin/pip_package/pip_package ./wheels

export PYTHON_BIN_PATH=python3.6
bazel build --copt="-D_GLIBCXX_USE_CXX11_ABI=0" //pip_package
bazel-bin/pip_package/pip_package ./wheels

...
```

## Build release with Linux docker container

```bash
docker run -it -v `pwd`:/tfunicode tensorflow/tensorflow:1.8.0-devel /tfunicode/pip_package/build_linux_release.sh
```

