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
        "@libsdl12//:sdlmain",
        "//glict/GLICT",
    ],
    data = [
        "@libsdl12//:sdlheaders",
        "font.bmp",
    ],
    copts = select({
        "//glict/GLICT:windows": ["-DBAZEL_WIN32"],
        "//glict/GLICT:windows_msys": [],
	"//glict/GLICT:windows_msvc": ["-DBAZEL_WIN32"],
        "//conditions:default": []
    }),
    linkopts = select({
        "//glict/GLICT:windows": ["-SUBSYSTEM:CONSOLE"],
        "//glict/GLICT:windows_msys": [],
	"//glict/GLICT:windows_msvc": ["-SUBSYSTEM:CONSOLE"],
        "//conditions:default": [
            "-ldl",
            "-pthread",
        ],
    }),
)

genrule(
    name = "font_bmp",
    srcs = ["font.bmp.gz"],
    outs = ["font.bmp"],
    cmd = "gunzip $(location font.bmp.gz) -c > $(@D)/font.bmp",
)
