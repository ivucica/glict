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


# With --define libsdl12_linux_deps_bin=true, a prebuilt set of x64 binary
# packages for dependencies will be used. Useful for remote builds where local
# headers and libs will not be installed.
#
# Ideally libsdl12 itself would offer us a way to link against the correct
# libraries such as libGL without having to pass this.
config_setting(
    name = "linux_deps_bin",
    values = {"define": "libsdl12_linux_deps_bin=true"},
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
    copts = select({
        # rules_libsdl12 needs system OpenGL headers.
        # Passed as copts since we know their path, but passing these -isystem paths in includes is not feasible as those paths are Bazel-package-relative.
        # (Hence they should be specified by libgl-dev instead, which is something that can be tackled in the future.)
        ":windows": [],
        ":windows_msys": [],
        ":windows_msvc": [],
        ":darwin": [],
        "//conditions:default": [],
        ":linux_deps_bin": [
            # Only use when rules_libsdl12 uses debs. Ideally we'd just pass on something where rules_libsdl12 would make the call. Not there yet.
            "-isystem",
            "external/libgl-dev/usr/include/",
        ],
    }),
    defines = select({
        ":windows": ["WIN32"],
        ":windows_msys": ["WIN32"],
        ":windows_msvc": ["WIN32"],
        "//conditions:default": [],
    }),
    includes = ["."],
    linkopts = select({
        ":windows": ["-lopengl32"],
        ":windows_msys": ["-lopengl32"],
        ":windows_msvc": ["-lopengl32"],
        ":darwin": ["-framework OpenGL"],
        "//conditions:default": ["-lGL"],
        ":linux_deps_bin": [
            #"-lGL", <-- it's libGL.so.1.7.0 inside libgl1, and we don't have a symlink to help find it
            # Ideally dep on libgl-dev should fix the search path, and maybe the
            # linking with libGL as well.
            "-Lexternal/libgl-dev/usr/lib/x86_64-linux-gnu/",
            "-Lexternal/libgl1/usr/lib/x86_64-linux-gnu/",
        ],
    }),
    visibility = ["//visibility:public"],
    deps = [
        ":glict_sys_headers",
    ] + select({
        ":windows": [],
        ":windows_msys": [],
        ":windows_msvc": [],
        ":darwin": [],
        "//conditions:default": [],
        ":linux_deps_bin": [
            # rules_libsdl12 can provide "system" OpenGL headers.

            # TODO: only use when rules_libsdl12 uses debs, without depending on the :linux_deps_bin.
            "@libgl-dev//:libgl-dev",  # Pull in GL/gl.h.
            "@libgl-dev//:hdrs",  # Pull in GL/gl.h.
            "@libgl1//:libs",  # Pull in libGL.so.1.7.0 as libGL.so symlinks to it and we have no other way to refer to it.
            "@x11repository//:X11",
        ],
    }),
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
    deps = select({
        "//conditions:default": [],
        ":linux_deps_bin": [
            "@libgl-dev//:hdrs",
            "@libgl-dev//:libs",
            "@libgl-dev//:libgl-dev",
        ],
    }),
    include_prefix = "GLICT",
)
