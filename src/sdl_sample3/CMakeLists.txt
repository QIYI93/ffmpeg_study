project(sdl_sample3)
message("sdl_sample3")

#set(FIND_MODE REQUIRED)


set(util_DIR "${MAINSOURCE}/src/util")
file (GLOB_RECURSE util
      "${util_DIR}/*.h*"
	  "${util_DIR}/*.c*")

set(SOURCES
sdlwindow.cpp
sdl_sample3.cpp)

set(HEADERS
sdlwindow.h)

include_directories(${SDL2}/include)
include_directories(${SDL2_image}/include)
include_directories(${SDL2_ttf}/include)

link_directories(${SDL2}/lib/x86)
link_directories(${SDL2_image}/lib/x86)
link_directories(${SDL2_ttf}/lib/x86)

add_executable(sdl_sample3 WIN32
	${util}
	${HEADERS}
	${SOURCES})

if(WIN32)
target_link_libraries(sdl_sample3 SDL2.lib)
target_link_libraries(sdl_sample3 SDL2main.lib)
target_link_libraries(sdl_sample3 SDL2_image.lib)
target_link_libraries(sdl_sample3 SDL2_ttf.lib)
endif()

# Copy DLLs to the output directory on Windows
if(WIN32)
add_custom_command(TARGET sdl_sample3 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL2}/lib/x86/SDL2.dll" ${CMAKE_BINARY_DIR}/rundir/Debug)
add_custom_command(TARGET sdl_sample3 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL2_image}/lib/x86/SDL2_image.dll" ${CMAKE_BINARY_DIR}/rundir/Debug)
add_custom_command(TARGET sdl_sample3 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL2_ttf}/lib/x86/libfreetype-6.dll" ${CMAKE_BINARY_DIR}/rundir/Debug)
add_custom_command(TARGET sdl_sample3 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL2_ttf}/lib/x86/SDL2_ttf.dll" ${CMAKE_BINARY_DIR}/rundir/Debug)
add_custom_command(TARGET sdl_sample3 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL2_ttf}/lib/x86/zlib1.dll" ${CMAKE_BINARY_DIR}/rundir/Debug)
endif()

if(MSVC)
set_target_properties(sdl_sample3 PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
endif()

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/rundir")
#target_link_libraries()
assign_source_group(${util})