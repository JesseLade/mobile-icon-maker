cmake_minimum_required(VERSION 3.15)
project(raylib_ios_app)

set(CMAKE_CXX_STANDARD 17)

# Set iOS SDK
set(CMAKE_OSX_SYSROOT iphoneos)
set(CMAKE_OSX_DEPLOYMENT_TARGET 12.0)
set(CMAKE_OSX_ARCHITECTURES "arm64")

# Raylib iOS path
set(RAYLIB_SRC raylib/src)
set(RAYLIB_PLATFORM src/platform/ios) # if you have platform-specific code

add_subdirectory(${RAYLIB_SRC} raylib_build)

include_directories(${RAYLIB_SRC} ${RAYLIB_SRC}/extras)

file(GLOB SOURCES "src/*.cpp")

add_executable(icon_maker ${SOURCES})
target_link_libraries(icon_maker raylib)
cmake_minimum_required(VERSION 3.14...3.27) # or whatever CMake version you have
project(icon_maker)

add_executable(icon_maker src/main.cpp)

