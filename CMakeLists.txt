cmake_minimum_required(VERSION 3.20)
project(sdltest4)

set(CMAKE_CXX_STANDARD 23)

add_executable(sdltest4 src/Main.cpp src/Application.h src/Application.cpp src/Audio.cpp src/Audio.h src/IoHelpers.cpp src/IoHelpers.h src/Texture.cpp)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(sdltest4 PRIVATE SDL2::SDL2 SDL2::SDL2main)

find_package(sdl2-mixer CONFIG REQUIRED)
target_link_libraries(sdltest4 PRIVATE SDL2::SDL2_mixer)
