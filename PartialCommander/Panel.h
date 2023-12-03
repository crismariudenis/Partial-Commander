#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include "Folder.h"

class Panel
{
    int width;
    int height;
    sf::Vector2f topLeft, botRight;
    std::vector<Folder> folders;
    int firstToDisplay, lastToDisplay;
    std::filesystem::path currentPath;
    void update(std::filesystem::path path);
    void drawFolders(sf::RenderWindow& mainWindow);
    void drawBorders(sf::RenderWindow& mainWindow);
    void drawColumnTitles(sf::RenderWindow& mainWindow);

public:
    
    void draw(sf::RenderWindow& mainWindow);
    Panel() {};
    void configure(int width, int height, std::filesystem::path currentPath, sf::Vector2f topLeft, sf::Vector2f botRight);

};