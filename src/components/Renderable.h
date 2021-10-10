#pragma once

#include "Texture.h"
#include "Sprite.h"
#include "SpriteSheet.h"

struct Renderable {
    int layer = 100;

    std::string  textureName;
    Texture*     texture;
    Sprite*      sprite;
    SpriteSheet* spriteSheet;
};
