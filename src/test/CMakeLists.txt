find_package(xencode)
find_package(xtransport)
find_package(xlog)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

aux_source_directory(. TEST_SRCS)

include_directories(${XENCODE_INCLUDE_DIR})
include_directories(${XTRANSPORT_INCLUDE_DIR})
include_directories(${XLOG_INCLUDE_DIR})

add_executable(xtest ${TEST_SRCS})

target_link_libraries(xtest ${XENCODE_LIBRARY} ${XTRANSPORT_LIBRARY})

add_custom_command(TARGET xtest
	POST_BUILD
    COMMAND ./test/xtest
	COMMAND gcovr -s -r .. --exclude-directories 'lib/transport/CMakeFiles/xtransport.dir/gen-cpp' 
	COMMAND gcovr -s -r .. --exclude-directories 'lib/transport/CMakeFiles/xtransport.dir/gen-cpp' --xml -o ${COVERAGE_DIR}/coverage.xml
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)

INSTALL(TARGETS xtest
                RUNTIME DESTINATION bin
)
