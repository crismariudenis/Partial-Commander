#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include "Folder.h"

class Panel
{
    int width, height;
    sf::Vector2f pos;
    std::vector<Folder> folders;
    sf::RenderWindow& mainWindow;
    int firstToDisplay, lastToDisplay, selectedFolderIndex;
    std::filesystem::path currentPath;
    std::vector<sf::Font> fonts;
    void update(std::filesystem::path path);
    void drawFolders();
    void drawBorders();
    void drawColumnTitles();
    void drawSelectedFolderBackground();
    void updateFoldersPosition(sf::Vector2f move);

    bool isSelected = false;

public:

    void draw();
    void init(sf::Vector2f pos, int width, int height, std::filesystem::path currentPath, std::vector<sf::Font>& fonts);
    Panel(sf::RenderWindow& mainWindow) :mainWindow(mainWindow) {};
    void updateSelectedFolder(sf::Keyboard::Scancode);
    void toggleIsSelected();
    void changePath();

};