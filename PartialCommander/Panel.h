#pragma once

#include "Scrollbar.h"
#include "Folder.h"

class Panel
{
    int width, height, sortType;
    sf::Vector2f pos;
    sf::RenderWindow& window;
    std::filesystem::path currentPath;
    std::vector<sf::Font> fonts;
    std::unordered_map<int, bool> filteredFolders;
    Scrollbar scrollbar;

    sf::Text folderName, sizeName, dateName;
    sf::Text searchText;
    sf::Text currentPathText;

    sf::RectangleShape panelRect, line1, line2, line3;

    std::vector<std::tuple<float, float, float, float>> quadrants;
    void drawFolders();
    void drawSelectedFolderBackground();
    void drawFreeSpace();
    void drawSearchText();
    void drawCurrentPath();
    void resetTextPositions(std::vector<sf::Vector2f> initialPositions);
    bool isValidByFilter(std::string folderText);
   

public:
    void updateFolderSelectedFolder(int newSelectedFolder);
    bool isSelected = false, isSearchActive = false, isDirectoryLabelActive = true;
    std::vector<Folder> folders, foldersCopy;
    int firstToDisplay, lastToDisplay, selectedFolderIndex, shiftSelectedFolder = -1;

    Panel(sf::RenderWindow& window) :window(window) {};
    void init(sf::Vector2f pos, int width, int height, std::filesystem::path currentPath, std::vector<sf::Font>& fonts);
    void initColumnTitles();
    void initBorders();
    void initCurrentPath();
    void updateColors();
    void draw();
    void updateSelectedFolder(sf::Keyboard::Scancode);
    void toggleIsSelected();
    void changePath(int type);
    void changeDirectory(std::filesystem::path directoryPath);
    void checkTextLabels(sf::Vector2f mouse);
    void checkFolderLabels(sf::Vector2f mouse);
    void activateLabel(int mouseX, int mouseY);
    void resetFoldersPositions();
    int getSelectedFolderIndex();
    void updateShortcutSelectedFolder(int type, int move);
    void updateFoldersByFilter();
    void pasteFromClipboard(std::vector<Folder> folders);
    bool checkMouseOnFolder(int index, float mouseX, float mouseY);
    bool checkScrollbarLabel(sf::Vector2f mouse);
    void updateByScrollbar(sf::Vector2f mouse);
    void updateByScrollbar(int steps);
    void registerCharacter(int scancode, bool isUpperCase, int type);
    Folder getSelectedFolder();
    void setSelectedFolder(std::string text);
    void activateSearch();
    std::vector<Folder> &getFolders();
    void update(std::filesystem::path path);
    std::filesystem::path getCurrentPath();
  };