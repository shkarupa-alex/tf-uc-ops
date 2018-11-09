load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("//tf:tf_configure.bzl", "tf_configure")

# curl -L https://github.com/.../.../archive/<git hash>.tar.gz | [g]sha256sum

http_archive(
    name = "re2",  # 2018-10-01
    sha256 = "ed26a822d723dde3493a7689a053c23f783d65811bca6e6084e51db9da639aa4",
    strip_prefix = "re2-0a6326bfc1de75e371d8ab1f776a6717521f95cd",
    urls = ["https://github.com/google/re2/archive/0a6326bfc1de75e371d8ab1f776a6717521f95cd.tar.gz"],
)

http_archive(
    name = "unilib",  # v3.1.1--devel + fresh unicode data
    sha256 = "77214520fb32733e91f47883ceb487962f5d161ea568d8f5baca5c0ff084936c",
    strip_prefix = "unilib-e74d98658cf504322d9aafa2bb52dd6a46b8dc1f",
    urls = ["https://github.com/shkarupa-alex/unilib/archive/e74d98658cf504322d9aafa2bb52dd6a46b8dc1f.tar.gz"],
    build_file = "BUILD.unilib",
)

tf_configure(
    name = "local_config_tf",
)
