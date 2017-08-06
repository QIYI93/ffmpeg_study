project(ffmepg_sample1)
message("ffmepg_sample1")

set(util_DIR "${MAINSOURCE}/src/util")
file (GLOB_RECURSE util
      "${util_DIR}/*.h*"
	  "${util_DIR}/*.c*")

set(SOURCES
ffmepg_sample1.cpp)

set(HEADER
header.h)

include_directories(${FFMPEG}/include)

link_directories(${FFMPEG}/lib/x86)

add_executable(ffmepg_sample1 WIN32
	${SOURCES}
	${util}
	${HEADER})

if(WIN32)
target_link_libraries(ffmepg_sample1 avdevice.lib avcodec.lib avfilter.lib avformat.lib avutil.lib postproc.lib swresample.lib swscale.lib)
endif()

# Disable SAFESEH on Windows
if(WIN32)
	SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
endif()

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/rundir")
assign_source_group(${util})

if(MSVC)
set_target_properties(ffmepg_sample1 PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
endif()

# Copy DLLs to the output directory on Windows
if(WIN32)
add_custom_command(TARGET ffmepg_sample1 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${FFMPEG}/bin/x86" ${CMAKE_BINARY_DIR}/rundir/Debug)
endif()