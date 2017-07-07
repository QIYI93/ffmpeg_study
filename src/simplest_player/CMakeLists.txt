project(simplest_player)
message("simplest_player")

#set(FIND_MODE REQUIRED)


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
link_directories(${FFMPEG}/lib)

add_executable(simplest_player WIN32
	${SOURCES}
	${util}
	${HEADER})

target_link_libraries(simplest_player SDL2.lib SDL2main.lib)
target_link_libraries(simplest_player avdevice.lib avcodec.lib avfilter.lib avformat.lib avutil.lib postproc.lib swresample.lib swscale.lib)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/rundir")
#target_link_libraries()
assign_source_group(${util})