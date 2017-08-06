project(ffmpeg_playing_sound)
message("ffmepg_playing_sound")

set(util_DIR "${MAINSOURCE}/src/util")
file (GLOB_RECURSE util
      "${util_DIR}/*.h*"
	  "${util_DIR}/*.c*")

set(SOURCES
ffmepg_playing_sound.cpp)

include_directories(${SDL2}/include)
include_directories(${FFMPEG}/include)

link_directories(${SDL2}/lib/x86)
link_directories(${FFMPEG}/lib/x86)

add_executable(ffmepg_playing_sound WIN32
	${SOURCES}
	${util})

if(WIN32)
target_link_libraries(ffmepg_playing_sound SDL2.lib SDL2main.lib)
target_link_libraries(ffmepg_playing_sound avdevice.lib avcodec.lib avfilter.lib avformat.lib avutil.lib postproc.lib swresample.lib swscale.lib)
endif()

# Disable SAFESEH on Windows
if(WIN32)
	SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
endif()

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/rundir")
assign_source_group(${util})

if(MSVC)
set_target_properties(ffmepg_playing_sound PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
endif()

# Copy DLLs to the output directory on Windows
if(WIN32)
add_custom_command(TARGET ffmepg_playing_sound POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL2}/lib/x86/SDL2.dll" ${CMAKE_BINARY_DIR}/rundir/Debug)
add_custom_command(TARGET ffmepg_playing_sound POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${FFMPEG}/bin/x86" ${CMAKE_BINARY_DIR}/rundir/Debug)
endif()