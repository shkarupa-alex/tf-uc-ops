load("@org_tensorflow//tensorflow:workspace.bzl", "tf_workspace")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def tfunicode_workspace():
    http_archive(
        name = "unilib",  # v3.1.1--devel + fresh unicode data
        sha256 = "77214520fb32733e91f47883ceb487962f5d161ea568d8f5baca5c0ff084936c",
        strip_prefix = "unilib-e74d98658cf504322d9aafa2bb52dd6a46b8dc1f",
        urls = ["https://github.com/shkarupa-alex/unilib/archive/e74d98658cf504322d9aafa2bb52dd6a46b8dc1f.tar.gz"],
        build_file = "BUILD.unilib",
    )

    tf_workspace(path_prefix = "", tf_repo_name = "org_tensorflow")
