new_http_archive(
    name = "libsdl12",
    url = "http://www.libsdl.org/release/SDL-1.2.15.tar.gz",
    sha256 = "d6d316a793e5e348155f0dd93b979798933fb98aa1edebcc108829d6474aad00",
    type = "tar.gz",
    build_file = "BUILD.libsdl12",
    strip_prefix = "SDL-1.2.15/",
)

# For work on libsdl rules only.
new_local_repository(
    name = "libsdl12_local",
    build_file = "BUILD.libsdl12",
    path = __workspace_dir__ + "/SDL-1.2.15/",
)

new_local_repository(
    name = "macx11",
    build_file_content = "exports_files(glob(['**']), visibility = ['//visibility:public'])",
    path = "/opt/X11/include",
)

workspace(name="glict")
