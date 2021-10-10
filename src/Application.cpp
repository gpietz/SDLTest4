#include "Application.h"
#include "DrawBuffer.h"
#include <SDL_image.h>
#include <iostream>
#include <format>
#include <systems/RenderSystem.h>
#include <systems/MoveSystem.h>
#include "Components.h"
#include "helpers/TimeHelper.h"
#include "SpriteSheet.h"

const auto TITLE          = "SDLTest4";
const auto SCREEN_WIDTH   = 1024;
const auto SCREEN_HEIGHT  = 768;
const auto IMG_INIT_FLAGS = IMG_INIT_JPG | IMG_INIT_PNG;

Application::Application() : m_fps(std::chrono::milliseconds(16)) {
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

    m_spawner.set(m_textures);
    m_spawner.set(m_renderContext);

    auto redHatRun = new SpriteSheet();
    if (!redHatRun->loadFromXmlFile("resources/gfx/redhatrun.xml", m_renderContext)) {
        std::cerr << "Failed loading spritesheet!" << std::endl;
        return;
    }
    m_renderContext->m_redHatRun = redHatRun;
    m_spawner.set(redHatRun);
}

Application::~Application() {
    delete m_renderContext;
    delete m_audio;
    delete m_textures;
    SDL_FreeSurface(m_window_surface);
    SDL_DestroyWindow(m_window);
}

void Application::run() {
    m_audio->playStreamed("resources/sfx/Juhani_Junkala-Stage2.ogg", true);
    m_textures->getTexture("resources/gfx/powered.png", m_renderer);
    m_textures->getTexture("resources/gfx/bjarne.jpg", m_renderer);
    m_textures->getTexture("resources/gfx/f1.png", m_renderer);
    update();
}

void Application::update() {
    Uint32 frameStart = 0, frameTime = 0, frameDelay = 50;

    auto last = std::chrono::steady_clock::now();
    while (m_keep_running) {
        const auto now = std::chrono::steady_clock::now();
        const auto dt  = std::chrono::duration_cast<FDuration>(now - last);
        if (dt < m_fps) {
            continue;
        }

        last = now;

        frameStart = SDL_GetTicks();
        m_spawner.update(m_registry, dt);
        MoveSystem::update(m_registry, m_renderContext, dt);
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

        if (TimeHelper::HasTimeExpired(m_lastWindowTitleUpdate, 1000)) {
            auto rocketCount = m_registry.view<Rocket>().size();
            auto newTitle = std::format("{} | Rockets #{}", TITLE, rocketCount);
            SDL_SetWindowTitle(m_window, newTitle.c_str());
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
    drawBuffer.submit(0);

    RenderSystem::draw(m_renderContext, m_registry);
    DrawBuffer::render(m_renderContext);
}

void Application::processKeyboardInput(SDL_KeyboardEvent *keyboardEvent) {
    switch (keyboardEvent->keysym.sym) {
        case SDLK_ESCAPE: // Escape pressed = exit application
            m_keep_running = false;
            break;
    }
}
