cmake_minimum_required(VERSION 3.15...3.27)
project(icon_maker)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_OSX_SYSROOT iphoneos)
set(CMAKE_OSX_DEPLOYMENT_TARGET 12.0)
set(CMAKE_OSX_ARCHITECTURES "arm64")

# Add Raylib source files directly
file(GLOB RAYLIB_SRC
    raylib/src/*.c
    raylib/src/extras/*.c
)

# Your app source
file(GLOB SOURCES "src/*.cpp")

# Include headers
include_directories(raylib/src raylib/src/extras)

add_executable(icon_maker ${SOURCES} ${RAYLIB_SRC})

# Link frameworks needed by raylib
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
