# -*-Python-*-
cc_binary(
    name = "sdldemo",
    srcs = [
        "sdldemo.cpp",
    ],
    deps = [
        "@libsdl12//:sdl",
        "//glict/GLICT",
    ],
    data = [
        "@libsdl12//:sdlheaders",
    ],
    linkopts = [
        "-ldl",
    ],
)
