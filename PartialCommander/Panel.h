#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include "Folder.h"

class Panel
{
    int width, height, sortType;
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
    void drawCurrentPath();
   

    void updateFoldersPosition(sf::Vector2f move);
    std::string getDate(std::filesystem::path p);

    bool checkBoxLabel(int topLeftX, int topLeftY, int botRightX, int botRightY, int mouseX, int mouseY);
    bool isSelected = false;

public:

    Panel(sf::RenderWindow& mainWindow) :mainWindow(mainWindow) {};
    void init(sf::Vector2f pos, int width, int height, std::filesystem::path currentPath, std::vector<sf::Font>& fonts);
    void draw();
    void updateSelectedFolder(sf::Keyboard::Scancode);
    void toggleIsSelected();
    void changePath();
    void checkTextLabels(int mouseX, int mouseY);
     void activateLabel(int mouseX, int mouseY);

};