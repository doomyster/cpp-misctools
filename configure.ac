AC_INIT([cpp-multitools], [0.1], [nnormand@justcodeit.org], [c++-tools], [https://github.com/doomyster/cpp-misctools])

AC_CONFIG_MACRO_DIR([m4])
LT_INIT
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])

AC_PROG_LIBTOOL
AC_PROG_CXX
AM_PATH_CPPUNIT
PKG_CHECK_MODULES([PCRECPP], [libpcrecpp])

AC_CONFIG_FILES([Makefile
src/Makefile
tests/Makefile])
AC_OUTPUT

