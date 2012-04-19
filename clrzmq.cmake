if(CMAKE_CL_64)
	set(LIBZMQ_TARGET_PATH "lib/x64")
else(CMAKE_CL_64)
	set(LIBZMQ_TARGET_PATH "lib/x86")
endif(CMAKE_CL_64)

if(MSVC_IDE)
	set(STAGE_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>")
else(MSVC_IDE)
	set(STAGE_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif(MSVC_IDE)

set(CLRZMQ_DLL "${CMAKE_CURRENT_SOURCE_DIR}/clrzmq/src/ZeroMQ/bin/$<CONFIGURATION>/clrzmq.dll")
set(MSBUILD "%SYSTEMROOT%\\Microsoft.NET\\Framework\\v4.0.30319\\msbuild")

execute_process(COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_CURRENT_SOURCE_DIR}/clrzmq/src/ZeroMQ/packages.config)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/nuged.exe clrzmq/src/.nuget
	
add_custom_target(clrzmq ALL 
	${CMAKE_COMMAND} -E copy "${STAGE_DIR}/libzmq.dll" "${LIBZMQ_TARGET_PATH}"
	COMMAND cmd ARGS "/c" ${MSBUILD} "src\\clrzmq.sln" "/target:Project\\ZeroMQ" "/p:Configuration=$<CONFIGURATION>"
	COMMAND ${CMAKE_COMMAND} -E copy "${CLRZMQ_DLL}" "${STAGE_DIR}/clrzmq.dll"
	DEPENDS libzmq
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/clrzmq
	)

install(FILES "${STAGE_DIR}/clrzmq.dll" DESTINATION bin)

