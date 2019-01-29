# tfunicode

Infrastructure to build TensorFlow custom ops wheel for unicode string preprocessing.
For more info about provided ops see package [README](https://github.com/shkarupa-alex/tfunicode/blob/master/pip_package/README.md)

## Develop environment
Install all [dependencies including python headers](https://www.tensorflow.org/install/install_sources).
Do not use pyenv in MacOS X, otherwise tests mostly likely will fail.

## Build PIP package manually
You can build the pip package with Bazel:
```bash
export PYTHON_BIN_PATH=`which python2.7`
$PYTHON_BIN_PATH -m pip install -U tensorflow  # Only if you did not install it yet
./configure.sh
bazel clean --expunge
bazel test --test_output=errors //tfunicode/...
bazel build build_pip_pkg
bazel-bin/build_pip_pkg wheels
```

## Build release with Linux docker container
```bash
docker run -it -v `pwd`:/tfunicode library/ubuntu:xenial /tfunicode/build_linux_release.sh
```

## Install and test PIP package
Once the pip package has been built, you can install it with:
```bash
pip install wheels/*.whl
```

Now you can test out the pip package:
```bash
cd /
python -c "import tensorflow as tf;import tfunicode as tfu;print(tfu.transform_zero_digits('123').eval(session=tf.Session()))"
```

You should see the op zeroed out all nonzero digits in string "123":
```bash
000
```


