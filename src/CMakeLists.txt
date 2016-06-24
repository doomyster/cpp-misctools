file (GLOB sources cppmt-encoding.cc cppmt-env.cc cppmt-getopt.cc cppmt-sqlite-cell_t.cc cppmt-sqlite-main.cc cppmt-sqlite-result_t.cc cppmt-sqlite-row_t.cc cppmt-string.cc cppmt-zlib.cc cppmt-endianness.cc)
file (GLOB headers cppmt-encoding.h  cppmt-env.h  cppmt-env.hxx  cppmt-exceptions.h  cppmt-getopt.h  cppmt-getopt.hxx  cppmt-sqlite.h  cppmt-sqlite.hxx  cppmt-string.h  cppmt-zlib.h  cppmt-zlib.hxx cppmt-endianness.h)

add_library (cpp-misctools SHARED ${sources})

target_link_libraries (cpp-misctools ${ZLIB_LIBRARIES})
target_link_libraries (cpp-misctools ${SQLITE3_LIBRARIES})
target_link_libraries (cpp-misctools ${PCRECPP_LIBRARIES})
target_link_libraries (cpp-misctools ${LIBB64_LIBRARIES})

target_include_directories (cpp-misctools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install (TARGETS cpp-misctools DESTINATION lib)
install (FILES ${headers} DESTINATION include/cpp-misctools)

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
