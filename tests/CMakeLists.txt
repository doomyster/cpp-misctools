file (GLOB sources cpptTest.cc cpptTest-string.cc cpptTest-zlib.cc cpptTest-encoding.cc cpptTest-getopt.cc cpptTest-getenv.cc cpptTest-sqlite.cc cpptTest-endianness.cc)

add_executable(cppmt-test ${sources})

include_directories (${CPPUNIT_INCLUDE_DIRS})
link_directories (cpp-misctools ${CPPUNIT_LIBRARIES})

target_link_libraries (cppmt-test ${CPPUNIT_LIBRARIES})
target_link_libraries (cppmt-test ${ZLIB_LIBRARIES})
target_link_libraries (cppmt-test ${SQLITE3_LIBRARIES})
target_link_libraries (cppmt-test ${PCRECPP_LIBRARIES})
target_link_libraries (cppmt-test ${LIBB64_LIBRARIES})
target_link_libraries (cppmt-test cpp-misctools)

target_include_directories (cppmt-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (cppmt-test PUBLIC ${CPP-MISCTOOLS_SOURCE_DIR}/src)

configure_file (tests_01.getopt.cfg            ${CMAKE_CURRENT_BINARY_DIR}                                COPYONLY)
configure_file (tests_02.getopt.cfg            ${CMAKE_CURRENT_BINARY_DIR}                                COPYONLY)
configure_file (data/Bare_mountain,_nevada.jpg ${CMAKE_CURRENT_BINARY_DIR}/data/Bare_mountain,_nevada.jpg COPYONLY)

add_test (NAME UnitTests COMMAND cppmt-test DEPENDS cppmt-test)

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

