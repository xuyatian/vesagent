find_library(XENCODE_LIBRARY xencode ${PROJECT_BINARY_DIR}/lib/encode)
IF(NOT DEFINED ${XENCODE_LIBRARY})
	set(XENCODE_LIBRARY ${PROJECT_BINARY_DIR}/lib/transport/libencode.so)
ENDIF()
find_path(XENCODE_INCLUDE_DIR XEvent.h ${CMAKE_SOURCE_DIR}/lib/encode/include)
message("xencode include dir: ${XENCODE_INCLUDE_DIR}")
message("xencode library dir: ${XENCODE_LIBRARY}")

