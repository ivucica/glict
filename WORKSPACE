# -*-Python-*-
workspace(name="glict")

load("//:x11-helper.bzl", "x11_repository")

new_http_archive(
    name = "libsdl12",
    url = "http://www.libsdl.org/release/SDL-1.2.15.tar.gz",
    sha256 = "d6d316a793e5e348155f0dd93b979798933fb98aa1edebcc108829d6474aad00",
    strip_prefix = "SDL-1.2.15/",

    # TODO(ivucica): cannot grab from VCS: no ./configure
    #url = "http://hg.libsdl.org/SDL/archive/0c1a8b0429a9.tar.gz",
    #sha256 = "f4be8ec4c1f465438b90395bf370ab7cb39fb3ffff9fc46f8f3941100de6acee",
    #strip_prefix = "SDL-0c1a8b0429a9",

    type = "tar.gz",
    build_file = "BUILD.libsdl12",
)

# For work on libsdl rules only.
new_local_repository(
    name = "libsdl12_local",
    build_file = "BUILD.libsdl12",
    path = __workspace_dir__ + "/SDL-1.2.15/",
)

x11_repository()


