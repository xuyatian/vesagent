cmake_minimum_required (VERSION 2.8)

project (xsample)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(xencode)
find_package(xtransport)
find_package(xlog)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

aux_source_directory(. SAMPLE_SRCS)

include_directories(${XENCODE_INCLUDE_DIR})
include_directories(${XTRANSPORT_INCLUDE_DIR})
include_directories(${XLOG_INCLUDE_DIR})

add_executable(xsample ${SAMPLE_SRCS})

target_link_libraries(xsample ${XENCODE_LIBRARY} ${XTRANSPORT_LIBRARY})

INSTALL(TARGETS xsample
                RUNTIME DESTINATION bin
)
