# -*-Python-*-
workspace(name="glict")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "rules_libsdl12",
    commit = "ff61b0a18827602cbc72b8bd823bdc70a59975d7",
    remote = "https://github.com/ivucica/rules_libsdl12",
)

load("@rules_libsdl12//:libsdl12.bzl", "libsdl12_repositories")
libsdl12_repositories()

