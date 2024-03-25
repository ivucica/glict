# -*-Python-*-
cc_binary(
    name = "memory_sdl",
    srcs = [
        "main.cpp",
        "sdlfont.cpp",
        "sdlfont.h",
    ],
    copts = select({
        "//glict/GLICT:windows": ["-DBAZEL_WIN32"],
        "//glict/GLICT:windows_msys": [],
        "//glict/GLICT:windows_msvc": ["-DBAZEL_WIN32"],
        "//conditions:default": [],
        "//glict/GLICT:linux_deps_bin": [
            "-isystem", "external/libgl-dev/usr/include/",
        ],
    }),
    data = [
        "font.bmp",
        "@libsdl12//:sdlheaders",
    ],
    linkopts = select({
        "//glict/GLICT:windows": ["-SUBSYSTEM:CONSOLE"],
        "//glict/GLICT:windows_msys": [],
        "//glict/GLICT:windows_msvc": ["-SUBSYSTEM:CONSOLE"],
        "//conditions:default": [
            "-ldl",
            "-pthread",
        ],
        "//glict/GLICT:linux_deps_bin": [
            "-ldl",
            "-pthread",
            # Ideally dep on libgl-dev should fix the search path, and maybe the
            # linking with libGL as well.
            # This should also be added only if rules_libsdl12 decided to use
            # .deb-derived binaries and headers.
            "-Lexternal/libgl-dev/usr/lib/x86_64-linux-gnu/",
        ],
    }),
    deps = [
        "//glict/GLICT",
        "@libsdl12//:sdl",
        "@libsdl12//:sdlmain",
    ] + select({
        "//conditions:default": [],
        "//glict/GLICT:linux_deps_bin": [
            "@libgl-dev//:hdrs",
            #"@libgl-dev//:libgl-dev",
            #"@libgl-dev//:libs",
            #"@libgl1//:libs",
            #"@libgl1//:libGL",
        ],
    }),
)

genrule(
    name = "font_bmp",
    srcs = ["font.bmp.gz"],
    outs = ["font.bmp"],
    cmd = "gunzip $(location font.bmp.gz) -c > $(@D)/font.bmp",
)
