# -*-Python-*-
cc_binary(
    name = "memory_sdl",
    srcs = [
        "main.cpp",
        "sdlfont.cpp",
        "sdlfont.h",
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
        "-pthread",
    ],
)
