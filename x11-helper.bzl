# -*-Python-*-
def x11_repository():
    return native.new_local_repository(
        name = "x11repository",
        build_file_content = "\n".join([
            "exports_files(glob(['X11/**']), ",
            "              visibility = ['//visibility:public'])",
            "genrule(",
            "    name=\"xext\",",
            "    srcs = glob([\"X11/extensions/*.h\"]),",
            "    outs = [i.replace(\"X11/extensions/\", \"Xext/extensions/\")",
            "            for i in glob([\"include/*.h\"])],",
            "    cmd = \"mkdir -p $(@D)/Xext/extensions; for i in $(SRCS) ; do cp \\\"$i\\\" \\\"\$\$(@D)/Xext/extensions/\$\$(basename \\\"${i}\\\")\\\"; done\",",
            ")",
        ]),
        path = "/usr/include",
    )

def xcb_repository():
    return native.new_local_repository(
        name = "xcbrepository",
        build_file_content = "\n".join([
            "exports_files(glob(['xcb/**']), ",
            "              visibility = ['//visibility:public'])",
        ]),
        path = "/usr/include",
    )
