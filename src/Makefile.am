lib_LTLIBRARIES = libcpp-misctools.la

libcpp_misctools_la_SOURCES  = cppmt-getopt.cc \
                               cppmt-zlib.cc \
			       cppmt-encoding.cc \
			       cppmt-string.cc
libcpp_misctools_la_includedir = $(includedir)/cpp-misctools
libcpp_misctools_la_include_HEADERS = cppmt-exceptions.h cppmt-getopt.h cppmt-getopt.hxx cppmt-string.h cppmt-misctools.h cppmt-zlib.h cppmt-zlib.hxx cppmt-encoding.h

libcpp_misctools_la_CXXFLAGS  = @PCRECPP_CFLAGS@ @ZLIB_CFLAGS@
libcpp_misctools_la_LIBADD    = @PCRECPP_LIBS@   @ZLIB_LIBS@ -lb64

libcpp_misctools_la_SOURCES += cppmt-sqlite-main.cc \
                               cppmt-sqlite-cell_t.cc \
			       cppmt-sqlite-result_t.cc \
			       cppmt-sqlite-row_t.cc

libcpp_misctools_la_include_HEADERS += cppmt-sqlite.h cppmt-sqlite.hxx

libcpp_misctools_la_CXXFLAGS += @SQLITE3_CFLAGS@
libcpp_misctools_la_LIBADD   += @SQLITE3_LIBS@

