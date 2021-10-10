#include "RenderSystem.h"
#include "Components.h"
#include "DrawBuffer.h"
#include "DrawCommand.h"
#include "Sprite.h"
#include <entt/entt.hpp>
#include <iostream>

// Forward declaration
float calculate_ease(float duration, float in, float out, float peak);

void RenderSystem::draw(const RenderContext* renderContext, entt::registry &registry)
{
    DrawBuffer drawBuffer;

    auto view = registry.view<const Renderable>();
    for (auto entity : view) {
        auto layer       = view.get<const Renderable>(entity).layer;
        auto textureName = view.get<const Renderable>(entity).textureName;
        auto texture     = view.get<const Renderable>(entity).texture;
        auto spriteSheet = view.get<const Renderable>(entity).spriteSheet;

        if (textureName.empty() && texture == nullptr && spriteSheet == nullptr) {
            std::cerr << "Renderable without texturename !" << std::endl;
            registry.remove<Renderable>(entity);
            return;
        }

        // Get texture form spritesheet
        if (texture == nullptr && spriteSheet != nullptr) {
            texture = renderContext->m_textureManager->getTexture(spriteSheet->getTexture().c_str());
        }

        // Get texture from texture-manager
        if (texture == nullptr) {
            texture = renderContext->m_textureManager->getTexture(textureName.c_str());
        }

        // Fading
        auto opacity = 1.0;
        auto fadeInOut = registry.try_get<FadeInOut>(entity);
        if (fadeInOut != nullptr) {
            if (fadeInOut->startTime == std::nullopt) {
                fadeInOut->startTime = SystemClock::now();
            }

            FDuration duration = SystemClock::now() - fadeInOut->startTime.value();
            auto millis  = std::chrono::duration_cast<Millis>(duration);
            if (millis.count() > fadeInOut->getTotalDuration()) {
                if (!fadeInOut->keepWhenDone) {
                    registry.remove<Renderable>(entity);
                    return;
                }
                registry.remove<FadeInOut>(entity);
            }
            else
            {
                opacity = calculate_ease(millis.count(),
                                         fadeInOut->durationIn, fadeInOut->durationOut, fadeInOut->durationPeak);
            }
        }

        auto position = registry.try_get<Position>(entity);

        if (spriteSheet == nullptr) {

            // Draw Sprite
            ////////////////

            auto sprite = new Sprite(texture, nullptr);
            auto drawSprite = new DrawCommands::DrawSprite(sprite);
            drawSprite->opacity = opacity;

            if (position != nullptr) {
                drawSprite->position = std::unique_ptr<SDL_Point>(new SDL_Point{
                    .x = position->x,
                    .y = position->y
                });
            }

            auto rocket = registry.try_get<Rocket>(entity);
            if (rocket != nullptr && rocket->speed < 0) {
                sprite->flipMode = SDL_FLIP_HORIZONTAL;
            }

            drawBuffer.addCommand(drawSprite);
        } else {

            // Draw SpriteSheet
            /////////////////////

            auto drawSpriteSheet = new DrawCommands::DrawSpriteSheet(spriteSheet);

            if (position != nullptr) {
                drawSpriteSheet->position = std::unique_ptr<SDL_Point>(new SDL_Point {
                    .x = position->x,
                    .y = position->y
                });
            }

            drawBuffer.addCommand(drawSpriteSheet);
        }
        drawBuffer.submit(layer);
    }
}

float calculate_ease(float duration, float in, float out, float peak)
{
    const float f1 = 1.0;

    if (duration >= in + out + peak)
        return 0;
    if (duration >= in && duration <= (in + peak))
        return f1;
    if (duration < in)
        return f1 / 100 * (duration / in * 100);

    duration -= (in + peak);
    return f1 - (f1 / 100 * (duration / out * 100));
}
