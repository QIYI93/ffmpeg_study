cmake_minimum_required(VERSION 2.8.12)

project(ffmpeg-study)

if(WIN32)
	set(QTDIR "D:/project/library/Qt/x86/Qt5.9.1/5.9.1/msvc2015")
	set(SDL2 "D:/project/ffmpeg/deps/SDL2-2.0.5")
	set(SDL2_image "D:/project/ffmpeg/deps/SDL2_image-2.0.1")
	set(SDL2_mixer "D:/project/ffmpeg/deps/SDL2_mixer-2.0.1")
	set(SDL2_net "D:/project/ffmpeg/deps/SDL2_net-2.0.1")
	set(SDL2_ttf "D:/project/ffmpeg/deps/SDL2_ttf-2.0.14")
	set(FFMPEG "D:/project/ffmpeg/deps/ffmpeg")
	set(MAINSOURCE ${PROJECT_SOURCE_DIR})
endif()

include_directories(${PROJECT_SOURCE_DIR}/src/util)

if(WIN32)
add_subdirectory(src)
endif()
