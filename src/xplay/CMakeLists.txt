project(xplay)
message("xplay")

set(util_DIR "${MAINSOURCE}/src/util")
file (GLOB_RECURSE util
      "${util_DIR}/*.h*"
	  "${util_DIR}/*.c*")

if(WIN32)
	if(DEFINED QTDIR)
		list(APPEND CMAKE_PREFIX_PATH "${QTDIR}")
	elseif(DEFINED ENV{${QTDIR}})
		list(APPEND CMAKE_PREFIX_PATH "$ENV{${QTDIR}}")
	else()
		message(FATAL_ERROR "unknown Qt version ...")
	endif()
else()
	message(FATAL_ERROR "unknown platform ...")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

if(NOT Qt5Widgets_FOUND)
	message(FATAL_ERROR "Failed to find Qt5")
	return()
endif()

if(NOT Qt5Network_FOUND)
	message(FATAL_ERROR "Failed to find Qt5Network")
	return()
endif()

set(SOURCES
main.cpp)

include_directories(${FFMPEG}/include)

link_directories(${FFMPEG}/lib/x86)

add_executable(xplay WIN32
	${SOURCES}
	${util})

if(WIN32)
	if(MSVC_VERSION EQUAL 1900)
		message(STATUS "MSVC_VERSION EQUAL 1900")
		target_link_libraries(xplay
		Qt5::Widgets
		Qt5::Network
		avdevice.lib
		avcodec.lib
		avfilter.lib
		avformat.lib
		avutil.lib
		postproc.lib
		swresample.lib
		swscale.lib)
	endif()
endif()

# Disable SAFESEH on Windows
if(WIN32)
	SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
endif()

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/rundir")
assign_source_group(${util})

if(MSVC)
set_target_properties(xplay PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
endif()

# Copy DLLs to the output directory on Windows
if(WIN32)
add_custom_command(TARGET xplay POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${FFMPEG}/bin/x86" ${CMAKE_BINARY_DIR}/rundir/Debug)
add_custom_command(TARGET xplay POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QTDIR}/bin/Qt5Widgetsd.dll" ${CMAKE_BINARY_DIR}/rundir/Debug)
add_custom_command(TARGET xplay POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QTDIR}/bin/Qt5Widgetsd.pdb" ${CMAKE_BINARY_DIR}/rundir/Debug)
add_custom_command(TARGET xplay POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QTDIR}/bin/Qt5Cored.dll" ${CMAKE_BINARY_DIR}/rundir/Debug)
add_custom_command(TARGET xplay POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QTDIR}/bin/Qt5Cored.pdb" ${CMAKE_BINARY_DIR}/rundir/Debug)
add_custom_command(TARGET xplay POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QTDIR}/bin/Qt5Guid.dll" ${CMAKE_BINARY_DIR}/rundir/Debug)
add_custom_command(TARGET xplay POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QTDIR}/bin/Qt5Guid.pdb" ${CMAKE_BINARY_DIR}/rundir/Debug)
endif()