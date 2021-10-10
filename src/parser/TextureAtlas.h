#pragma once

#include <string>
#include <vector>
#include "rapidxml/rapidxml.hpp"

class TextureAtlas {
public:
    TextureAtlas();
    ~TextureAtlas();

    bool loadFromFile(const char* filename);
    std::string& getImagePath() noexcept;
    int getWidth() noexcept;
    int getHeight() noexcept;
    void getSize(int& width, int& height) noexcept;
    std::string& getError() noexcept;

    struct Sprite {
        std::string Name;
        int xPos;
        int yPos;
        int width;
        int height;
        int xPivot;
        int yPivot;
        int xOffset;
        int yOffset;
        int oWidth;
        int oHeight;
    };

    typedef std::unique_ptr<Sprite> SpritePtr;
    std::vector<SpritePtr> sprites;

private:
    std::string m_error;
    std::string m_imagePath;
    int m_width, m_height;

    bool createSpriteNode(rapidxml::xml_node<>* node);
    Sprite* parseSprite(rapidxml::xml_node<>* node);
    char* getAttribute(rapidxml::xml_node<>* node, const char* attributeName) const;
    int getAttributeAsInt(rapidxml::xml_node<>* node, const char* attributeName, int defaultValue = -1) const;
    void storeError(const char* errorMessage) noexcept;
};
