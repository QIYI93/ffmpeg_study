project(sdl_sample1)
message("sdl_sample1")

#set(FIND_MODE REQUIRED)


set(util_DIR "${MAINSOURCE}/src/util")
file (GLOB_RECURSE util
      "${util_DIR}/*.h*"
	  "${util_DIR}/*.c*")

set(SOURCES
sdl_sample1.cpp)

include_directories(${SDL2}/include)

link_directories(${SDL2}/lib/x86)


add_executable(sdl_sample1 WIN32
	${SOURCES}
	${util})

target_link_libraries(sdl_sample1 SDL2.lib)
target_link_libraries(sdl_sample1 SDL2main.lib)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/rundir")
#target_link_libraries()
assign_source_group(${util})