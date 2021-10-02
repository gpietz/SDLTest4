#include "Application.h"

const auto TITLE         = "SDLTest4";
const auto SCREEN_WIDTH  = 1024;
const auto SCREEN_HEIGHT = 768;

Application::Application() {
    m_keep_running = true;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "Failed to initialize the SDL2 librrary (damn!)" << std::endl;
        std::cout << "SDL2 Error: " << SDL_GetError() << std::endl;
        return;
    }

    m_window = SDL_CreateWindow(TITLE,
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                SCREEN_WIDTH, SCREEN_HEIGHT,
                                0);
    if (!m_window) {
        std::cout << "Failed to create window!" << std::endl;
        std::cout << "SDL2 Error: " << SDL_GetError() << std::endl;
        return;
    }

    m_window_surface = SDL_GetWindowSurface(m_window);
    if (!m_window_surface) {
        std::cout << "Failed to get the surface from the window!" << std::endl;
        std::cout << "SDL2 Error: " << SDL_GetError() << std::endl;
        return;
    }

    m_audio = std::unique_ptr<Audio>(new Audio());
}

Application::~Application() {
    SDL_FreeSurface(m_window_surface);
    SDL_DestroyWindow(m_window);
}

void Application::run() {
    m_audio->playStreamed("resources/sfx/Juhani_Junkala-Stage2.ogg");
    update();
}

void Application::update() {
    while (m_keep_running) {
        while (SDL_PollEvent(&m_window_event) > 0) {
            switch (m_window_event.type) {
                case SDL_QUIT:
                    m_keep_running = false;
                    break;

                case SDL_KEYDOWN: // Escape pressed
                    processKeyboardInput(&m_window_event.key);
                    break;
            }
        }

        draw();
    }
}

void Application::draw() {
    SDL_UpdateWindowSurface(m_window);
}

void Application::processKeyboardInput(SDL_KeyboardEvent *keyboardEvent) {
    switch (keyboardEvent->keysym.sym) {
        case SDLK_ESCAPE: // Escape pressed = exit application
            m_keep_running = false;
            break;
    }
}

// Just a helper function; should be place somewhere else some day.
SDL_Surface* load_surface(const char *path) {
    auto image_surface = SDL_LoadBMP(path);

    if (!image_surface)
        return 0;

    return image_surface;
}
