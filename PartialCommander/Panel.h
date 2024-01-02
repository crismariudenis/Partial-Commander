#pragma once

#include "Clipboard.h"
#include "Scrollbar.h"
#include "System.h"

class Panel
{
    int width, height, sortType;
    sf::Vector2f pos;
    sf::RenderWindow& window;
    std::filesystem::path currentPath;
    std::vector<sf::Font> fonts;
    std::unordered_map<int, bool> shortcutSelectedFolder, filteredFolders;
    Scrollbar scrollbar;
    System* sys = System::getInstance();

    sf::Text folderName, sizeName, dateName;
    sf::Text searchText;
    sf::Text currentPathText;

    sf::RectangleShape panelRect, line1, line2, line3;

    std::vector<std::tuple<float, float, float, float>> quadrants;

    void update(std::filesystem::path path);
    void drawFolders();
    void drawBorders();
    void drawColumnTitles();
    void drawSelectedFolderBackground();
    void drawCurrentPath();
    void drawFreeSpace();
    void drawScrollbar();
    void drawSearchText();

    std::string getDate(std::filesystem::path p);

    bool checkBoxLabel(float topLeftX, float topLeftY, float botRightX, float botRightY, float mouseX, float mouseY);
    void resetTextPositions(std::vector<sf::Vector2f> initialPositions);
    void updateFolderSelectedFolder(int newSelectedFolder);

    bool isValidByFilter(std::string folderText);

public:
    bool isSelected = false, isSearchActive = false, isDirectoryLabelActive = true;
    std::vector<Folder> folders, foldersCopy;
    int firstToDisplay, lastToDisplay, selectedFolderIndex, shiftSelectedFolder = -1;

    Panel(sf::RenderWindow& window) :window(window) {};
    void init(sf::Vector2f pos, int width, int height, std::filesystem::path currentPath, std::vector<sf::Font>& fonts);
    void initColumnTitles();
    void initBorders();
    void initCurrentPath();

    void draw();
    void updateSelectedFolder(sf::Keyboard::Scancode);
    void toggleIsSelected();
    void changePath();
    void changeDirectory(std::filesystem::path directoryPath);
    void checkTextLabels(sf::Vector2f mouse);
    void checkFolderLabels(sf::Vector2f mouse);
    void activateLabel(int mouseX, int mouseY);
    void resetFoldersPositions();
    int getSelectedFolderIndex();
    void updateShortcutSelectedFolder(int type, int move);
    void updateFoldersByFilter();
    void updateClipboard();
    void pasteFromClipboard(std::vector<Folder> folders);
    bool checkMouseOnFolder(int index, float mouseX, float mouseY);
    bool checkScrollbarLabel(sf::Vector2f mouse);
    void updateByScrollbar(sf::Vector2f mouse);
    void updateByScrollbar(int steps);
    void registerCharacter(int scancode, bool isUpperCase, int type);
    Folder getSelectedFolder();
    void setSelectedFolder(std::string text);
    void activateSearch();
    void deactivateSearch();
    void rename(std::string oldName);


   };