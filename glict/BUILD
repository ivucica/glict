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
        "//conditions:default": [],
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
