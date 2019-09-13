cmake_minimum_required (VERSION 2.8)

project (xvesagent)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x") 

OPTION (ENABLE_COVERAGE "Use coverage" OFF)
MESSAGE(STATUS ENABLE_COVERAGE=${ENABLE_COVERAGE})

iF(NOT DEFINED COVERAGE_DIR)
	set(COVERAGE_DIR ".")
ENDIF()

MESSAGE(STATUS COVERAGE_DIR=${COVERAGE_DIR})

IF(ENABLE_COVERAGE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
	add_subdirectory(test)
	add_custom_target(cov
    	COMMAND ./test/xtest
		COMMAND lcov -c -d ${CMAKE_BINARY_DIR}/lib/encode/CMakeFiles/xencode.dir -d ${CMAKE_BINARY_DIR}/lib/transport/CMakeFiles/xtransport.dir -o coverage.info
		COMMAND lcov -r coverage.info '/usr/*' '*/transport/gen-cpp/*' -o ${COVERAGE_DIR}/xvesagent.info
		COMMAND genhtml xvesagent.info -o ${COVERAGE_DIR}coverage
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    	)
	add_dependencies(cov xencode xtransport xtest)
	add_dependencies(xtest xencode xtransport)
ENDIF()

add_subdirectory(lib)

