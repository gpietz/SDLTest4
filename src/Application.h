#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <entt/entity/registry.hpp>
#include "Audio.h"
#include "TextureManager.h"
#include "RenderContext.h"
#include "Systems/SpawnerSystem.h"

class Application {
public:
    Application();
    ~Application();

    void run();

private:
    const FDuration   m_fps;
    SDL_Window*     m_window;
    SDL_Surface*    m_window_surface;
    SDL_Renderer*   m_renderer;
    SDL_Event       m_window_event { };
    bool            m_keep_running;
    RenderContext*  m_renderContext;
    SystemTimePoint m_lastWindowTitleUpdate;

    // Subsystems
    Audio*          m_audio;
    TextureManager* m_textures;
    entt::registry  m_registry;

    // ECS Systems
    SpawnerSystem   m_spawner;

    void update();
    void draw();
    void processKeyboardInput(SDL_KeyboardEvent *keyboardEvent);
};
