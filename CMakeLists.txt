cmake_minimum_required(VERSION 3.15...3.27)
project(icon_maker)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_OSX_SYSROOT iphoneos)
set(CMAKE_OSX_DEPLOYMENT_TARGET 12.0)
set(CMAKE_OSX_ARCHITECTURES "arm64")

include_directories(raylib/src raylib/src/extras)

file(GLOB SOURCES "src/*.cpp")

add_executable(icon_maker ${SOURCES})

target_link_libraries(icon_maker ${CMAKE_SOURCE_DIR}/raylib/build-ios/libraylib.a)
