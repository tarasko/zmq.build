include_directories(${ZMQ_INCLUDE_DIR})

file(GLOB sources RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")
file(GLOB headers RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.hpp")

foreach(src ${sources})
	string(REGEX REPLACE "(.*)\\.cpp" "example.\\1" target ${src})
	add_executable(${target} ${src} ${headers})
	target_link_libraries(${target} zmq)
	install(TARGETS ${target} DESTINATION bin)
endforeach(src)
