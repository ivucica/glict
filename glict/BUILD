# -*-Python-*-
config_setting(
    name = "darwin",
    values = {"host_cpu": "darwin"}
)

cc_binary(
    name = "sdldemo",
    srcs = [
        "sdldemo.cpp",
    ],
    deps = [
        "@libsdl12//:sdl",
        "//glict/GLICT",
    ] + select({
        ":darwin": ["@libsdl12//:sdlmain"],
        "//glict/GLICT:linux_deps_bin": ["@libgl1//:libgl1", "@libgl-dev//:libgl-dev"],  # TODO: remove if possible, linking with :sdl should do this
        "//conditions:default": [],
    }),
    copts = select({
        "//conditions:default": [],
        "//glict/GLICT:linux_deps_bin": ["-isystem", "external/libgl-dev/usr/include"],  # TODO: remove if possible, linking with :sdl should do this
    }),
    data = [
        "@libsdl12//:sdlheaders",
    ],
    linkopts = [
        "-pthread"
    ] + select({
        "//conditions:default": [
            "-ldl",
        ],
        ":darwin": [],
    })
)
