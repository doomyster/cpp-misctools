TESTS = cppt-test
check_PROGRAMS = $(TESTS)

cppt_test_SOURCES = cpptTest.cc \
                    cpptTest-string.cc \
		    cpptTest-zlib.cc \
		    cpptTest-getopt.cc

cppt_test_CXXFLAGS = $(CPPUNIT_CFLAGS) @PCRECPP_CFLAGS@ -I $(top_srcdir)/src
cppt_test_LDFLAGS  = $(CPPUNIT_LIBS)   @PCRECPP_LIBS@      $(top_builddir)/src/libcpp-misctools.la -lz


