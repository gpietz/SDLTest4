#include "Application.h"
#include <SDL_image.h>

#include <memory>

#include "DrawBuffer.h"

const auto TITLE         = "SDLTest4";
const auto SCREEN_WIDTH  = 1024;
const auto SCREEN_HEIGHT = 768;
const auto IMG_INIT_FLAGS = IMG_INIT_JPG | IMG_INIT_PNG;

Application::Application() {
    m_keep_running = true;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "Failed to initialize the SDL2 library (damn!)" << std::endl;
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

    // Initialize support for PNG and JPG loading
    if (!(IMG_Init(IMG_INIT_FLAGS) & IMG_INIT_FLAGS)) {
        std::cerr << "Failed to initialize SDL Image library!" << std::endl;
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        return;
    }

    m_window_surface = SDL_GetWindowSurface(m_window);
    if (!m_window_surface) {
        std::cout << "Failed to get the surface from the window!" << std::endl;
        std::cout << "SDL2 Error: " << SDL_GetError() << std::endl;
        return;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        std::cerr << "Failed to create renderer!" << std::endl;
        std::cerr << "SDL2 Error: " << SDL_GetError() << std::endl;
        return;
    }

    m_audio = new Audio();
    m_textures = new TextureManager();
    m_renderContext = new RenderContext(m_renderer, m_textures);
}

Application::~Application() {
    SDL_FreeSurface(m_window_surface);
    SDL_DestroyWindow(m_window);
}

void Application::run() {
    m_audio->playStreamed("resources/sfx/Juhani_Junkala-Stage2.ogg", true);
    m_textures->getTexture("resources/gfx/bjarne.jpg", m_renderer);
    update();
}

void Application::update() {
    Uint32 frameStart = 0, frameTime = 0, frameDelay = 50;
    while (m_keep_running) {
        frameStart = SDL_GetTicks();
        draw();

        while (SDL_PollEvent(&m_window_event) > 0) {
            switch (m_window_event.type) {
                case SDL_QUIT:
                    m_keep_running = false;
                    break;
                case SDL_KEYDOWN:
                    processKeyboardInput(&m_window_event.key);
                    break;
            }
        }

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void Application::draw() {
    auto drawBuffer = DrawBuffer();
    drawBuffer.addCommand(new DrawCommands::ClearScreen(12, 0, 12, 255));
    drawBuffer.addCommand(new DrawCommands::DrawImage("resources/gfx/bjarne.jpg"));
    drawBuffer.submit(100);

    DrawBuffer::render(m_renderContext);
}

void Application::processKeyboardInput(SDL_KeyboardEvent *keyboardEvent) {
    switch (keyboardEvent->keysym.sym) {
        case SDLK_ESCAPE: // Escape pressed = exit application
            m_keep_running = false;
            break;
    }
}
