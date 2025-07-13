cmake_minimum_required(VERSION 3.15...3.27)
project(icon_maker)

set(CMAKE_CXX_STANDARD 17)

# Set iOS SDK
set(CMAKE_OSX_SYSROOT iphoneos)
set(CMAKE_OSX_DEPLOYMENT_TARGET 12.0)
set(CMAKE_OSX_ARCHITECTURES "arm64")

# Raylib iOS path
set(RAYLIB_SRC raylib/src)
set(RAYLIB_PLATFORM src/platform/ios) # if you have platform-specific code

# Add raylib subdirectory (make sure it exists!)
add_subdirectory(${RAYLIB_SRC} raylib_build)

include_directories(${RAYLIB_SRC} ${RAYLIB_SRC}/extras)

# Gather your source files
file(GLOB SOURCES "src/*.cpp")

# Add your executable once, linking raylib
add_executable(icon_maker ${SOURCES})
target_link_libraries(icon_maker raylib)
