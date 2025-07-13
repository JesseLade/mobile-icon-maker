cmake_minimum_required(VERSION 3.15...3.27)
project(icon_maker)

set(CMAKE_CXX_STANDARD 17)

# REMOVE THESE — let toolchain handle it:
# set(CMAKE_OSX_SYSROOT iphoneos)
# set(CMAKE_OSX_DEPLOYMENT_TARGET 12.0)
# set(CMAKE_OSX_ARCHITECTURES "arm64")

file(GLOB SOURCES "src/*.cpp")
file(GLOB RAYLIB_SRC raylib/src/*.c raylib/src/extras/*.c)

include_directories(raylib/src raylib/src/extras)

add_executable(icon_maker ${SOURCES} ${RAYLIB_SRC})

target_link_libraries(icon_maker
    "-framework OpenGLES"
    "-framework UIKit"
    "-framework Foundation"
    "-framework AVFoundation"
    "-framework CoreAudio"
    "-framework CoreGraphics"
    "-framework QuartzCore"
    "-framework GameController"
)
