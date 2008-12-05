AC_INIT(GLICT, 1.1, ivucica@gmail.com)
AC_CONFIG_SRCDIR(GLICT/container.cpp)

PACKAGE=GLICT
VERSION=1.1.0



GLICT_RELEASE=1:1:0
GLICT_LIBRARY_VERSION=0:0:0
#                     | | |
#              +------+ | +---+
#              |        |     |
#            current:revision:age
#              |        |     |
#              |        |     +- increment if interfaces have been added
#              |        |        set to zero if interfaces have been removed
#              |        |        or changed
#              |        +- increment if source code has changed
#              |           set to zero if current is incremented
#              +- increment if interfaces have been added, removed or changed



AM_INIT_AUTOMAKE($PACKAGE, $VERSION)

AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB

AC_OUTPUT(Makefile GLICT/Makefile)

