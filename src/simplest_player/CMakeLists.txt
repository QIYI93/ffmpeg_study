project(simplest_player)
message("simplest_player")

set(util_DIR "${MAINSOURCE}/src/util")
file (GLOB_RECURSE util
      "${util_DIR}/*.h*"
	  "${util_DIR}/*.c*")

set(SOURCES
simplest_player.cpp)

set(HEADER
header.h)

include_directories(${SDL2}/include)
include_directories(${FFMPEG}/include)

link_directories(${SDL2}/lib/x86)
link_directories(${FFMPEG}/lib/x86)

add_executable(simplest_player WIN32
	${SOURCES}
	${util}
	${HEADER})

if(WIN32)
target_link_libraries(simplest_player SDL2.lib SDL2main.lib)
target_link_libraries(simplest_player avdevice.lib avcodec.lib avfilter.lib avformat.lib avutil.lib postproc.lib swresample.lib swscale.lib)
endif()

# Disable SAFESEH on Windows
if(WIN32)
	SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
endif()

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/rundir")
assign_source_group(${util})

# Copy DLLs to the output directory on Windows
if(WIN32)
add_custom_command(TARGET simplest_player POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL2}/lib/x86/SDL2.dll" ${CMAKE_BINARY_DIR}/rundir/Debug)
add_custom_command(TARGET simplest_player POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${FFMPEG}/bin/x86" ${CMAKE_BINARY_DIR}/rundir/Debug)
endif()