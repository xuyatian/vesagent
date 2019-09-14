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
	COMMAND lcov -c -d ${CMAKE_BINARY_DIR}/lib/encode/CMakeFiles/xencode.dir -d ${CMAKE_BINARY_DIR}/lib/transport/CMakeFiles/xtransport.dir -o coverage.info
	COMMAND lcov -r coverage.info '/usr/*' '*/transport/gen-cpp/*' -o ${COVERAGE_DIR}/xvesagent.info
	COMMAND genhtml ${COVERAGE_DIR}/xvesagent.info -o ${COVERAGE_DIR}/coverage
	COMMAND genhtml ${COVERAGE_DIR}/xvesagent.info -o ${COVERAGE_DIR}/
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)

INSTALL(TARGETS xtest
                RUNTIME DESTINATION bin
)
