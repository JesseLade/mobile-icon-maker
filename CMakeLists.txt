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
# Remove add_subdirectory(raylib/src)

include_directories(raylib/src)

file(GLOB SOURCES "src/*.cpp")

add_executable(icon_maker ${SOURCES})
target_link_libraries(icon_maker ${CMAKE_SOURCE_DIR}/raylib/build-ios/libraylib.a)


include_directories(${RAYLIB_SRC} ${RAYLIB_SRC}/extras)

# Gather your source files
file(GLOB SOURCES "src/*.cpp")

# Add your executable once, linking raylib
add_executable(icon_maker ${SOURCES})
target_link_libraries(icon_maker raylib)
