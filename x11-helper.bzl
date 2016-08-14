# -*-Python-*-
def x11_repository():
    return native.new_local_repository(
        name = "x11repository",
        build_file_content = ("exports_files(glob(['X11/**']), " +
                              "visibility = ['//visibility:public'])"),
        path = "/usr/include",
    )

