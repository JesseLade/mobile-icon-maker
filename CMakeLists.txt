cmake_minimum_required(VERSION 3.15)
project(icon_maker)

set(CMAKE_CXX_STANDARD 17)

# iOS Deployment Settings
set(CMAKE_OSX_SYSROOT iphoneos)
set(CMAKE_OSX_DEPLOYMENT_TARGET 12.0)
set(CMAKE_OSX_ARCHITECTURES "arm64")

# Raylib
set(RAYLIB_SRC raylib/src)
add_subdirectory(${RAYLIB_SRC} raylib_build)
include_directories(${RAYLIB_SRC} ${RAYLIB_SRC}/extras)

# Your Source Files
file(GLOB SOURCES "src/*.cpp")
add_executable(icon_maker ${SOURCES})
target_link_libraries(icon_maker raylib)
