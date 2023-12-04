#pragma once

#include <filesystem>
#include <SFML/Graphics.hpp>
#include "Config.h"

class Folder
{
private:

    std::filesystem::path folderPath, parentPath;
    std::uintmax_t size;
    std::string modifyTime;
    sf::Vector2f position;
    sf::Text folderText, sizeText;
    sf::Font font;


    std::string normalizePath(std::string path);
    std::string toString(std::uintmax_t size);


public:
    ~Folder() {};
    Folder(std::filesystem::path folderPath, sf::Vector2f position, std::filesystem::path parentPath, std::vector<sf::Font> &fonts);
    sf::Text getFolderText();
    sf::Text getSizeText();
    void initText(std::vector<sf::Font>& fonts);
};