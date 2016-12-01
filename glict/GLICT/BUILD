# -*-Python-*-

cc_library(
    name = "GLICT",
    srcs = [
        "button.cpp",
        "container.cpp",
        "fonts.cpp",
        "globals.cpp",
        "list.cpp",
        "messagebox.cpp",
        "panel.cpp",
        "progressbar.cpp",
        "scrollbar.cpp",
        "skinner.cpp",
        "textbox.cpp",
        "window.cpp",
    ],
    hdrs = [
        "button.h",
        "container.h",
        "fonts.h",
        "globals.h",
        "image.h",
        "list.h",
        "messagebox.h",
        "panel.h",
        "progressbar.h",
        "scrollbar.h",
        "skinner.h",
        "textbox.h",
        "types.h",
        "window.h",
    ],
    # make #include <GLICT/foo.h> work by passing parent to -isystem
    includes = [".."],
    visibility = ["//visibility:public"],

    defines = [
        "NO_GL",
    ],
)
