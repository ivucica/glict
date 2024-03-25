# -*-Python-*-
workspace(name = "glict")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "rules_libsdl12",
    commit = "14dd7d24caa2d8d3b1ef77186a187702c8df1c12",
    remote = "https://github.com/ivucica/rules_libsdl12",
)

load("@rules_libsdl12//:libsdl12.bzl", "libsdl12_repositories")
libsdl12_repositories()

