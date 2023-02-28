TESTS = cppt-test
check_PROGRAMS = $(TESTS)

cppt_test_SOURCES = cpptTest.cc \
                    cpptTest-string.cc \
		    cpptTest-zlib.cc \
		    cpptTest-endianness.cc \
		    cpptTest-encoding.cc \
		    cpptTest-getopt.cc \
		    cpptTest-getenv.cc \
		    cpptTest-sqlite.cc

cppt_test_CXXFLAGS = $(CPPUNIT_CFLAGS) @PCRECPP_CFLAGS@ @ZLIB_CFLAGS@ -I $(top_srcdir)/src
cppt_test_LDFLAGS  = $(CPPUNIT_LIBS)   @PCRECPP_LIBS@   @ZLIB_LIBS@      $(top_builddir)/src/libcpp-misctools.la -lb64


