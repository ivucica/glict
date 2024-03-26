# -*-Python-*-
workspace(name = "glict")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "rules_libsdl12",
    commit = "a0e9423ea35481d83b35712d249d17b243f55600",
    remote = "https://github.com/ivucica/rules_libsdl12",
)

load("@rules_libsdl12//:libsdl12.bzl", "libsdl12_repositories")
libsdl12_repositories()

