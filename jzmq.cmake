# CMake build script for OMQ Java bindings on Windows

cmake_minimum_required (VERSION 2.8)

project (JZMQ)
find_package (Java REQUIRED)
find_package (JNI REQUIRED)

find_program (JNI_JAVAH
	NAMES javah
	HINTS ${_JAVA_HINTS}
	PATHS ${_JAVA_PATHS}
)

#-----------------------------------------------------------------------------
# platform specifics

add_definitions(
	-DWIN32
	-DDLL_EXPORT
	-DFD_SETSIZE=1024
)

#-----------------------------------------------------------------------------
# source files

set(java-sources
	org/zeromq/ZMQ.java
	org/zeromq/ZMQException.java
	org/zeromq/ZMQForwarder.java
	org/zeromq/ZMQQueue.java
	org/zeromq/ZMQStreamer.java
	org/zeromq/EmbeddedLibraryTools.java
	org/zeromq/App.java
)
set(java-classes
	org/zeromq/ZMQ.class
	org/zeromq/ZMQ$$Context.class
	org/zeromq/ZMQ$$Socket.class
	org/zeromq/ZMQ$$Poller.class
	org/zeromq/ZMQ$$Error.class
	org/zeromq/ZMQException.class
	org/zeromq/ZMQQueue.class
	org/zeromq/ZMQForwarder.class
	org/zeromq/ZMQStreamer.class
	org/zeromq/EmbeddedLibraryTools.class
	org/zeromq/App.class
)
set(javah-headers
	org_zeromq_ZMQ.h
	org_zeromq_ZMQ_Error.h
	org_zeromq_ZMQ_Context.h
	org_zeromq_ZMQ_Socket.h
	org_zeromq_ZMQ_Poller.h
)
set(cxx-sources
	Context.cpp
	Poller.cpp
	Socket.cpp
	util.cpp
	ZMQ.cpp
)

add_definitions(
	-DZMQ_HAVE_OPENPGM
)

include_directories(
    src
	${CMAKE_CURRENT_BINARY_DIR}
	${JNI_INCLUDE_DIRS}
	${ZMQ_INCLUDE_DIR}
)

#-----------------------------------------------------------------------------
# source generators

foreach (source ${cxx-sources})
	list(APPEND sources ${CMAKE_CURRENT_SOURCE_DIR}/src/${source})
endforeach()

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/config.hpp
	COMMAND ${CMAKE_COMMAND}
	ARGS	-E
		copy
		${CMAKE_CURRENT_SOURCE_DIR}/builds/msvc/config.hpp
		${CMAKE_CURRENT_BINARY_DIR}/config.hpp
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/builds/msvc/config.hpp
)
list(APPEND sources ${CMAKE_CURRENT_BINARY_DIR}/config.hpp)

add_custom_command(
	OUTPUT ${javah-headers}
	COMMAND ${JNI_JAVAH}
	ARGS	-jni
		-classpath ${CMAKE_CURRENT_BINARY_DIR}
		org.zeromq.ZMQ
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS ${java-classes}
)
list(APPEND sources ${javah-headers})

set (source-tmp "")
foreach (source ${java-sources})
	list (APPEND source-tmp ${CMAKE_CURRENT_SOURCE_DIR}/src/${source})
endforeach()
set (java-sources ${source-tmp})

add_custom_command(
	OUTPUT ${java-classes}
	COMMAND ${JAVA_COMPILE}
	ARGS	-classpath ${CMAKE_CURRENT_BINARY_DIR}
		-sourcepath ${CMAKE_CURRENT_SOURCE_DIR}/src
		-d ${CMAKE_CURRENT_BINARY_DIR}
		${java-sources}
	DEPENDS ${java-sources}
)

add_custom_command(
	OUTPUT zmq.jar
	COMMAND ${JAVA_ARCHIVE}
	ARGS	cf
		zmq.jar
		${java-classes}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS ${java-classes}
)
list(APPEND sources zmq.jar)

#-----------------------------------------------------------------------------
# output

add_library(jzmq SHARED ${sources})
target_link_libraries(jzmq zmq)

set(docs
        AUTHORS
	COPYING
	COPYING.LESSER
	ChangeLog
	INSTALL
	NEWS
	README
	README-PERF
)

install (TARGETS jzmq RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/zmq.jar DESTINATION bin)
# install (FILES ${docs} DESTINATION doc)

# end of file