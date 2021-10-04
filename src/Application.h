#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "Audio.h"
#include "TextureManager.h"
#include "RenderContext.h"

class Application {
public:
    Application();
    ~Application();

    void run();

private:
    SDL_Window*     m_window;
    SDL_Surface*    m_window_surface;
    SDL_Renderer*   m_renderer;
    SDL_Event       m_window_event { };
    bool            m_keep_running;
    RenderContext*  m_renderContext;

    // Subsystems
    Audio*          m_audio;
    TextureManager* m_textures;

    void update();
    void draw();
    void processKeyboardInput(SDL_KeyboardEvent *keyboardEvent);
};
