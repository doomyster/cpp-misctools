cmake_minimum_required (VERSION 3.0.2)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

project (CPPMT)

find_package (ZLIB    REQUIRED)
find_package (SQLITE3 REQUIRED)
find_package (PCRECPP REQUIRED)
find_package (LIBB64  REQUIRED)
find_package (CPPUNIT REQUIRED)

include_directories (${ZLIB_INCLUDE_DIRS})
include_directories (${SQLITE3_INCLUDE_DIRS})
include_directories (${PCRECPP_INCLUDE_DIRS})
include_directories (${LIBB64_INCLUDE_DIRS})

link_directories (CPPMT ${ZLIB_LIBRARIES})
link_directories (CPPMT ${SQLITE3_LIBRARIES})
link_directories (CPPMT ${PCRECPP_LIBRARIES})
link_directories (CPPMT ${LIBB64_LIBRARIES})

configure_file ("cpp-misctools-config.in" "cpp-misctools-config" @ONLY)
configure_file ("cpp-misctools.pc.in" "cpp-misctools.pc" @ONLY)
install (FILES ${CMAKE_BINARY_DIR}/cpp-misctools-config DESTINATION bin           PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ)
install (FILES ${CMAKE_BINARY_DIR}/cpp-misctools.pc     DESTINATION lib/pkgconfig PERMISSIONS               OWNER_WRITE OWNER_READ               GROUP_READ               WORLD_READ)

add_subdirectory (src)

enable_testing ()
add_subdirectory (tests)

get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

