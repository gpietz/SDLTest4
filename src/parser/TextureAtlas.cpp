#include <iostream>
#include <fstream>
#include "TextureAtlas.h"

TextureAtlas::TextureAtlas() : m_error(), m_imagePath(), m_width(0), m_height(0) {
}

TextureAtlas::~TextureAtlas() {
    sprites.clear();
}

// Several ways to load a text file, but this is the simplest for me.
inline std::string loadTextFile(const char* file_name){
    std::ifstream text_file(file_name);
    std::string str((std::istreambuf_iterator<char>(text_file)),
                    (std::istreambuf_iterator<char>()));
    return str;
}

bool TextureAtlas::loadFromFile(const char* filename) {
    std::string xml = loadTextFile(filename);

    rapidxml::xml_document<> doc;
    doc.parse<0>(const_cast<char*>(xml.c_str()));

    rapidxml::xml_node<>* rootNode = doc.first_node("TextureAtlas");
    if (rootNode == nullptr) {
        storeError("Unable to find node TextureAtlas in xml file!");
        return false;
    }

    m_imagePath = getAttribute(rootNode, "imagePath");
    if (m_imagePath.empty()) {
        storeError("Missing imagePath in TextureAtlas node!");
        return false;
    }

    m_width  = getAttributeAsInt(rootNode, "width");
    m_height = getAttributeAsInt(rootNode, "height");

    auto spriteNode = rootNode->first_node();
    if (spriteNode != nullptr && createSpriteNode(spriteNode)) {
        while ((spriteNode = spriteNode->next_sibling()) != nullptr) {
            createSpriteNode(spriteNode);
        }
    }

    return !sprites.empty();
}

bool TextureAtlas::createSpriteNode(rapidxml::xml_node<>* node) {
    auto sprite = parseSprite(node);
    if (sprite != nullptr && sprite->width > 0 && sprite->height > 0) {
        sprites.push_back(SpritePtr(sprite));
        return true;
    }
    return false;
}

TextureAtlas::Sprite* TextureAtlas::parseSprite(rapidxml::xml_node<>* node) {
    if (strcmp(node->name(), "sprite") != 0)
        return nullptr;

    auto* sprite = new Sprite();
    sprite->Name    = getAttribute(node, "n");
    sprite->xPos    = getAttributeAsInt(node, "x");
    sprite->yPos    = getAttributeAsInt(node, "y");
    sprite->width   = getAttributeAsInt(node, "w");
    sprite->height  = getAttributeAsInt(node, "h");
    sprite->xPivot  = getAttributeAsInt(node, "pX");
    sprite->yPivot  = getAttributeAsInt(node, "pY");
    sprite->xOffset = getAttributeAsInt(node, "oX");
    sprite->yOffset = getAttributeAsInt(node, "oY");
    sprite->oWidth  = getAttributeAsInt(node, "oW");
    sprite->oHeight = getAttributeAsInt(node, "oH");
    return sprite;
}

char* TextureAtlas::getAttribute(rapidxml::xml_node<>* node, const char* attributeName) const {
    for (rapidxml::xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
        if (strcmp(attr->name(), attributeName) == 0) {
            return attr->value();
        }
    }
    return nullptr;
}

int TextureAtlas::getAttributeAsInt(rapidxml::xml_node<>* node, const char* attributeName, int defaultValue) const {
    auto attributeValue = getAttribute(node, attributeName);
    return attributeValue != nullptr ? std::stoi(attributeValue) : defaultValue;
}

std::string& TextureAtlas::getImagePath() noexcept {
    return m_imagePath;
}

int TextureAtlas::getWidth() noexcept {
    return m_width;
}

int TextureAtlas::getHeight() noexcept {
    return m_height;
}

void TextureAtlas::getSize(int& width, int& height) noexcept {
    width  = m_width;
    height = m_height;
}

void TextureAtlas::storeError(const char* errorMessage) noexcept {
    m_error = std::string(errorMessage);
    std::cerr << errorMessage << std::endl;
}

std::string& TextureAtlas::getError() noexcept {
    return m_error;
}
