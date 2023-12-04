#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include "Folder.h"

class Panel
{
    int width, height;
    sf::Vector2f pos;
    sf::Vector2f topLeft, botRight;
    std::vector<Folder> folders;
    sf::RenderWindow& mainWindow;
    int firstToDisplay, lastToDisplay;
    std::filesystem::path currentPath;
    void update(std::filesystem::path path);
    void drawFolders();
    void drawBorders();
    void drawColumnTitles();

public:

    void draw();
    void init(sf::Vector2f pos, int width, int height, std::filesystem::path currentPath);
    Panel(sf::RenderWindow& mainWindow) :mainWindow(mainWindow) {};
    void configure(int width, int height, std::filesystem::path currentPath, sf::Vector2f topLeft, sf::Vector2f botRight);

};