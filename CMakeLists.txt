cmake_minimum_required(VERSION 3.20)
project(sdltest4)

set(CMAKE_CXX_STANDARD 23)

add_executable(sdltest4 src/Main.cpp src/Application.h src/Application.cpp src/Audio.cpp src/Audio.h src/helpers/IoHelpers.cpp src/helpers/IoHelpers.h src/TextureManager.h src/core/HandleManager.h src/core/Handle.h src/core/HandleManager.h src/TextureManager.cpp src/DrawBuffer.cpp src/DrawBuffer.h src/DrawCommand.cpp src/DrawCommand.h src/helpers/SdlHelpers.cpp src/helpers/SdlHelpers.h src/RenderContext.cpp src/RenderContext.h src/Texture.cpp src/Texture.h src/core/Assert.h)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(sdltest4 PRIVATE SDL2::SDL2 SDL2::SDL2main)

find_package(sdl2-mixer CONFIG REQUIRED)
target_link_libraries(sdltest4 PRIVATE SDL2::SDL2_mixer)

find_package(sdl2-image CONFIG REQUIRED)
target_link_libraries(sdltest4 PRIVATE SDL2::SDL2_image)

add_definitions(-DBOOST_NO_WARN_NEW_VERSIONS=1)
add_definitions(-DBOOST_UUID_FORCE_AUTO_LINK)

find_package(Boost 1.77.0 COMPONENTS random)
target_link_libraries(sdltest4 PRIVATE ${Boost_LIBRARIES})
target_include_directories(sdltest4 PRIVATE ${Boost_INCLUDE_DIRS})
