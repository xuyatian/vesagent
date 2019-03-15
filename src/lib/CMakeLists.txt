add_subdirectory(encode)
if(BUILD_TRANSPORT)
	add_subdirectory(transport)
endif()

file(GLOB COM_HDRS "common/*.h")

install(FILES ${COM_HDRS}
	DESTINATION "include/xvesagent" 
)

install(FILES ../cmake/xlogConfig.cmake
	DESTINATION "lib/cmake/xlog" 
)
