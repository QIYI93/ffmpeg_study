project(sdl_sample2)
message("sdl_sample2")

#set(FIND_MODE REQUIRED)


set(util_DIR "${MAINSOURCE}/src/util")
file (GLOB_RECURSE util
      "${util_DIR}/*.h*"
	  "${util_DIR}/*.c*")

set(SOURCES
sdl_sample2.cpp)

include_directories(${SDL2}/include)
include_directories(${SDL2_image}/include)

link_directories(${SDL2}/lib/x86)
link_directories(${SDL2_image}/lib/x86)


add_executable(sdl_sample2 WIN32
	${SOURCES}
	${util})

target_link_libraries(sdl_sample2 SDL2.lib)
target_link_libraries(sdl_sample2 SDL2main.lib)
target_link_libraries(sdl_sample2 SDL2_image.lib)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/rundir")
#target_link_libraries()
assign_source_group(${util})