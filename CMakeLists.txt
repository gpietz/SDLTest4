cmake_minimum_required(VERSION 3.20)
project(sdltest4)

set(CMAKE_CXX_STANDARD 23)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(sdltest4 src/Main.cpp
                        src/Application.h
                        src/Application.cpp
                        src/Audio.cpp
                        src/Audio.h
                        src/helpers/IoHelpers.cpp
                        src/helpers/IoHelpers.h
                        src/TextureManager.h
                        src/core/HandleManager.h
                        src/core/Handle.h
                        src/core/HandleManager.h
                        src/TextureManager.cpp
                        src/DrawBuffer.cpp
                        src/DrawBuffer.h
                        src/DrawCommand.cpp
                        src/DrawCommand.h
                        src/helpers/SdlHelpers.cpp
                        src/helpers/SdlHelpers.h
                        src/RenderContext.cpp
                        src/RenderContext.h
                        src/Texture.cpp
                        src/Texture.h
                        src/core/Assert.h
                        src/Sprite.cpp
                        src/Sprite.h
                        src/SpriteSheet.cpp
                        src/SpriteSheet.h
                        src/components/Renderable.h
                        src/systems/RenderSystem.cpp
                        src/systems/RenderSystem.h
                        src/components/Position.h
                        src/Components.h
                        src/DrawHelper.cpp
                        src/DrawHelper.h
                        src/systems/SpawnerSystem.cpp
                        src/systems/SpawnerSystem.h
                        src/components/Locator.h
                        src/components/FadeInOut.h
                        src/systems/MoveSystem.cpp
                        src/systems/MoveSystem.h
                        src/helpers/TimeHelper.cpp
                        src/helpers/TimeHelper.h
                        src/parser/TextureAtlas.h
                        src/parser/TextureAtlas.cpp
        src/components/RedHatRun.h)

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

find_package(EnTT CONFIG REQUIRED)
target_link_libraries(sdltest4 PRIVATE EnTT::EnTT)

find_path(RAPIDXML_INCLUDE_DIRS "rapidxml/rapidxml.hpp")
target_include_directories(sdltest4 PRIVATE ${RAPIDXML_INCLUDE_DIRS})
