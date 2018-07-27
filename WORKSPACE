workspace(name = "tfunicode")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")


# To update TensorFlow to a new revision.
# 1. Update the TENSORFLOW_GIT_COMMIT args below to include the new git hash.
# 2. Get the sha256 hash of the archive with a command such as...
#    curl -L https://github.com/tensorflow/tensorflow/archive/<git hash>.tar.gz | sha256sum
#    and update the TENSORFLOW_SHA_256 with the result.
# 3. Check if http_archive "io_bazel_rules_closure" below is the same as in your TensorFlow commit

TENSORFLOW_GIT_COMMIT = "93bc2e2072e0daccbcff7a90d397b704a9e8f778" # v1.8.0
TENSORFLOW_SHA_256 = "21445b2a7e0c4f0ba24e80425a26e1f59c98f818bdebe89b9274e73eff52b12d"

http_archive(
    name = "org_tensorflow",
    sha256 = TENSORFLOW_SHA_256,
    strip_prefix = "tensorflow-%s" % TENSORFLOW_GIT_COMMIT,
    urls = [
        "https://mirror.bazel.build/github.com/tensorflow/tensorflow/archive/%s.tar.gz" % TENSORFLOW_GIT_COMMIT,
        "https://github.com/tensorflow/tensorflow/archive/%s.tar.gz" % TENSORFLOW_GIT_COMMIT,
    ],
)


# TensorFlow depends on "io_bazel_rules_closure" so we need this here.
# Need to be kept in sync with the same target in TensorFlow's WORKSPACE file.
http_archive(
    name = "io_bazel_rules_closure",
    sha256 = "a38539c5b5c358548e75b44141b4ab637bba7c4dc02b46b1f62a96d6433f56ae",
    strip_prefix = "rules_closure-dbb96841cc0a5fb2664c37822803b06dab20c7d1",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_closure/archive/dbb96841cc0a5fb2664c37822803b06dab20c7d1.tar.gz",
        "https://github.com/bazelbuild/rules_closure/archive/dbb96841cc0a5fb2664c37822803b06dab20c7d1.tar.gz",  # 2018-04-13
    ],
)


# Specify the minimum required bazel version.
load("@org_tensorflow//tensorflow:version_check.bzl", "check_bazel_version_at_least")

check_bazel_version_at_least("0.15.0")


# Add all new dependencies in workspace.bzl.
load("//tfunicode:workspace.bzl", "tfunicode_workspace")

tfunicode_workspace()


