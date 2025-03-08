# -*-Python-*-
workspace(name = "glict")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "rules_libsdl12",
    commit = "867c04bd6d51153d947870a130a53b1c08129934",
    remote = "https://github.com/ivucica/rules_libsdl12",
)

load("@rules_libsdl12//:libsdl12.bzl", "libsdl12_repositories")

libsdl12_repositories()

# Toolchains for remote execution via buildbuddy.
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "io_buildbuddy_buildbuddy_toolchain",
    sha256 = "1cab6ef3ae9b4211ab9d57826edd4bbc34e5b9e5cb1927c97f0788d8e7ad0442",
    strip_prefix = "buildbuddy-toolchain-b043878a82f266fd78369b794a105b57dc0b2600",
    urls = ["https://github.com/buildbuddy-io/buildbuddy-toolchain/archive/b043878a82f266fd78369b794a105b57dc0b2600.tar.gz"],
)

load("@io_buildbuddy_buildbuddy_toolchain//:deps.bzl", "buildbuddy_deps")

buildbuddy_deps()

load("@io_buildbuddy_buildbuddy_toolchain//:rules.bzl", "buildbuddy", "UBUNTU20_04_IMAGE")

buildbuddy(name = "buildbuddy_toolchain", container_image = UBUNTU20_04_IMAGE)
