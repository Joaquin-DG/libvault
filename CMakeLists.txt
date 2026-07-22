cmake_minimum_required(VERSION 3.16)

project(libvault LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_path(RAYLIB_INCLUDE_DIR
	NAMES raylib.h
	PATHS /usr/local/include
	REQUIRED
)

find_library(RAYLIB_LIBRARY
	NAMES raylib libraylib
	PATHS /usr/local/lib
	REQUIRED
)

add_executable(libvault
	src/main.cpp
)

target_include_directories(libvault PRIVATE ${RAYLIB_INCLUDE_DIR})
target_link_libraries(libvault PRIVATE ${RAYLIB_LIBRARY})

target_link_libraries(libvault PRIVATE m pthread dl rt)

