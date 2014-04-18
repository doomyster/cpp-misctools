lib_LTLIBRARIES = libcpp-misctools.la

libcpp_misctools_la_SOURCES = cppmt-getopt.cc \
			      cppmt-string.cc

libcpp_misctools_la_includedir = $(includedir)/cpp-misctools
libcpp_misctools_la_include_HEADERS = cppmt-exceptions.h cppmt-getopt.h cppmt-getopt.hxx cppmt-string.h cppmt-misctools.h

libcpp_misctools_la_CXXFLAGS = @PCRECPP_CFLAGS@
libcpp_misctools_la_LIBADD   = @PCRECPP_LIBS@

