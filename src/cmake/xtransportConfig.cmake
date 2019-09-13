find_library(XTRANSPORT_LIBRARY xtransport ${PROJECT_BINARY_DIR}/lib/transport)
IF(NOT DEFINED ${XTRANSPORT_LIBRARY})
	set(XTRANSPORT_LIBRARY ${PROJECT_BINARY_DIR}/lib/transport/libtransport.so)
ENDIF()
find_path(XTRANSPORT_INCLUDE_DIR XTransport.h ${CMAKE_SOURCE_DIR}/lib/transport/include)
message("xtransport include dir: ${XTRANSPORT_INCLUDE_DIR}")
message("xtransport library dir: ${XTRANSPORT_LIBRARY}")

