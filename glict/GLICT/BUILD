# -*-Python-*-

config_setting(
    name = "windows",
    values = {"cpu": "x64_windows"},
    visibility = ["//:__subpackages__"],
)

config_setting(
    name = "windows_msys",
    values = {"cpu": "x64_windows_msys"},
    visibility = ["//:__subpackages__"],
)

config_setting(
    name = "windows_msvc",
    values = {"cpu": "x64_windows_msvc"},
    visibility = ["//:__subpackages__"],
)

config_setting(
    name = "darwin",
    values = {"cpu": "darwin"},
    visibility = ["//:__subpackages__"],
)

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
    includes = ["."],
    visibility = ["//visibility:public"],

    defines = select({
        ":windows": ["WIN32"],
	":windows_msys": ["WIN32"],
	":windows_msvc": ["WIN32"],
        "//conditions:default": [],
    }),
    linkopts = select({
        ":windows": ["-lopengl32",],
        ":windows_msys": ["-lopengl32",],
        ":windows_msvc": ["-lopengl32",],
        ":darwin": ["-framework OpenGL"],
        "//conditions:default": ["-lGL"],
    }),
    deps = ["glict_sys_headers"],
)

# make #include <GLICT/foo.h> work
cc_library(
    name = "glict_sys_headers",
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
    include_prefix = "GLICT",
)
