include_directories(${ZMQ_INCLUDE_DIR})

file(GLOB sources RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")

foreach(src ${sources})
	string(REGEX REPLACE "(.*)\\.cpp" "\\1" target ${src})
	message(${target})
	add_executable(${target} ${src})
	target_link_libraries(${target} zmq)
	install(TARGETS ${target} DESTINATION bin)
endforeach(src)
