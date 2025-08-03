
cmake_minimum_required(VERSION 3.15...3.27)
project(mobile-icon-maker)

cmake_minimum_required(VERSION 3.15)
project(RaylibIconMaker LANGUAGES CXX)
>>>>>>> Initial commit: Raylib Icon Maker

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


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
=======
add_executable(raylib_icon_maker src/main.cpp)

# Try to find Raylib via pkg-config (easy on Linux/Mac Homebrew/MacPorts),
# or user may set RAYLIB_DIR or install Raylib as a system package.
find_package(raylib 4.0 REQUIRED)

target_link_libraries(raylib_icon_maker PRIVATE raylib)

# If raylib is not found, support fallback to pkg-config
if(NOT raylib_FOUND)
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(RAYLIB raylib)
        if(RAYLIB_FOUND)
            target_include_directories(raylib_icon_maker PRIVATE ${RAYLIB_INCLUDE_DIRS})
            target_link_directories(raylib_icon_maker PRIVATE ${RAYLIB_LIBRARY_DIRS})
            target_link_libraries(raylib_icon_maker PRIVATE ${RAYLIB_LIBRARIES})
        else()
            message(FATAL_ERROR "Could not find raylib via pkg-config or CMake find_package")
        endif()
    endif()
endif()

# macOS specifics: link frameworks if needed
if(APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)
    find_library(AUDIO_LIBRARY AudioToolbox REQUIRED)
    target_link_libraries(raylib_icon_maker PRIVATE
        ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY} ${AUDIO_LIBRARY}
    )
endif()
