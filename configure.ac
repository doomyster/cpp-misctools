AC_INIT([cpp-misctools], m4_esyscmd([tr -d '\n' < VERSION]), [nnormand@justcodeit.org], [c++-misctools], [https://github.com/doomyster/cpp-misctools])

AC_CONFIG_MACRO_DIR([m4])
LT_INIT
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])

AC_PROG_LIBTOOL
AC_PROG_CXX
AM_PATH_CPPUNIT
PKG_CHECK_MODULES([PCRECPP], [libpcrecpp])
PKG_CHECK_MODULES([SQLITE3], [sqlite3])

VERSION_MAJOR=m4_esyscmd([sed -e 's/\..*//g' VERSION])
VERSION_MINOR=m4_esyscmd([sed -e 's/[^.]*\.//1' -e 's/\..*//g' VERSION])
VERSION_REVISION=m4_esyscmd([sed -e 's/.*\.//g' VERSION])

AC_SUBST(VERSION_MAJOR)
AC_SUBST(VERSION_MINOR)
AC_SUBST(VERSION_REVISION)
AC_SUBST(HAVE_SQLITE3)

AC_CONFIG_FILES([cpp-misctools-config
cpp-misctools.pc
Makefile
src/Makefile
src/cppmt-misctools.h
tests/Makefile])
AC_OUTPUT

