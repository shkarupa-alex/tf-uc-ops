load("@org_tensorflow//tensorflow:workspace.bzl", "tf_workspace")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def tfunicode_workspace():
    http_archive(
        name = "icu",
        sha256 = "3dd9868d666350dda66a6e305eecde9d479fb70b30d5b55d78a1deffb97d5aa3",
        strip_prefix = "icu",
        urls = ["http://download.icu-project.org/files/icu4c/62.1/icu4c-62_1-src.tgz"],
        build_file = "BUILD.icu",
    )

    http_archive(
        name = "unilib",  # v3.1.1
        sha256 = "395408a3dc9c3db2b5c200b8722a13a60898c861633b99e6e250186adffd1370",
        strip_prefix = "unilib-0bc49faef8b2e44433387cdab7e980feb5cc867b",
        urls = ["https://github.com/tensorflow/tensorflow/archive/0bc49faef8b2e44433387cdab7e980feb5cc867b.tar.gz"],
        build_file = "BUILD.unilib",
    )

    tf_workspace(path_prefix = "", tf_repo_name = "org_tensorflow")
