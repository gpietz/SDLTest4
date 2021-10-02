#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "Audio.h"

class Application {
public:
    Application();
    ~Application();

    void run();

private:
    SDL_Window*  m_window;
    SDL_Surface* m_window_surface;
    SDL_Event    m_window_event;
    bool         m_keep_running;

    std::unique_ptr<Audio> m_audio;

    void update();
    void draw();
    void processKeyboardInput(SDL_KeyboardEvent *keyboardEvent);
};
