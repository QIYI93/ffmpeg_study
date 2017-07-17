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

if(WIN32)
target_link_libraries(sdl_sample1 SDL2.lib)
target_link_libraries(sdl_sample1 SDL2main.lib)
endif()

# Copy DLLs to the output directory on Windows
if(WIN32)
add_custom_command(TARGET sdl_sample1 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL2}/lib/x86/SDL2.dll" ${CMAKE_BINARY_DIR}/rundir/Debug)
endif()

if(MSVC)
set_target_properties(sdl_sample1 PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
#set_target_properties(sdl_sample1 PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
endif()

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/rundir")
#target_link_libraries()
assign_source_group(${util})