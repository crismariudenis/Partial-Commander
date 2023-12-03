#include "Panel.h"

void Panel::configure(int width, int height, std::filesystem::path currentPath, sf::Vector2f topLeft, sf::Vector2f botRight) {
    this->width = width;
    this->height = height;
    this->currentPath = currentPath;
    this->topLeft = topLeft;
    this->botRight = botRight;
    update(currentPath);
}

void Panel::drawFolders(sf::RenderWindow& mainWindow) {
    for (int index = firstToDisplay; index <= lastToDisplay; ++index) {
        mainWindow.draw(folders[index].getFolderText());
        mainWindow.draw(folders[index].getSizeText());
    }
}

void Panel::drawBorders(sf::RenderWindow& mainWindow) {

    ///Outside Borders
    drawLine(topLeft.x, topLeft.y - PANEL_OFFSET, botRight.x, topLeft.y - PANEL_OFFSET, 3.5, mainWindow, 0);
    drawLine(botRight.x, topLeft.y - PANEL_OFFSET, botRight.x + (botRight.y - topLeft.y) + PANEL_OFFSET, topLeft.y - PANEL_OFFSET, 3.5, mainWindow, 90);
    drawLine(topLeft.x, botRight.y, botRight.x, botRight.y, 3.5, mainWindow, 0);
    drawLine(topLeft.x, topLeft.y - PANEL_OFFSET, topLeft.x + botRight.y - topLeft.y + PANEL_OFFSET, topLeft.y - PANEL_OFFSET, 3.5, mainWindow, 90);

    ///Inside Borders
    drawLine(FOLDER_SPACE + topLeft.x - 25, topLeft.y, FOLDER_SPACE + topLeft.x - 25 + botRight.y - topLeft.y - PANEL_BOTTOM_HEIGHT, topLeft.y, 1.5, mainWindow, 90);
    drawLine(FOLDER_SPACE + topLeft.x - 25 + SIZE_SPACE, topLeft.y, FOLDER_SPACE + topLeft.x - 25 + botRight.y - topLeft.y - PANEL_BOTTOM_HEIGHT + SIZE_SPACE, topLeft.y, 1.5, mainWindow, 90);

    ///Bottom Area Borders
    drawLine(topLeft.x, botRight.y - PANEL_BOTTOM_HEIGHT, botRight.x, botRight.y - PANEL_BOTTOM_HEIGHT, 3.5, mainWindow, 0);
}

void Panel::drawColumnTitles(sf::RenderWindow& mainWindow) {

    sf::Text folderName;
    folderName.setString("Name");
    folderName.setCharacterSize(CHARACTER_SIZE);
    folderName.setFillColor(columnColor);
    folderName.setFont(fonts[3]);
    folderName.setPosition(sf::Vector2f(FOLDER_SPACE / 2 + topLeft.x - 40, topLeft.y - 25));
    folderName.setStyle(1);

    mainWindow.draw(folderName);

    sf::Text sizeName = folderName;
    sizeName.setString("Size");
    sizeName.setPosition(sf::Vector2f(FOLDER_SPACE + SIZE_SPACE / 2 + topLeft.x - 50, topLeft.y - 25));
    mainWindow.draw(sizeName);

}

void Panel::draw(sf::RenderWindow& mainWindow) {
    drawFolders(mainWindow);
    drawBorders(mainWindow);
    drawColumnTitles(mainWindow);
}

void Panel::update(std::filesystem::path path) {

    this->currentPath = path;
    sf::Vector2f textPosition = topLeft;
    textPosition.x += 10;

    firstToDisplay = 0, lastToDisplay = 1;
    folders.push_back(Folder("/..", textPosition, currentPath));
    for (auto const& entry : std::filesystem::directory_iterator(currentPath)) {
        textPosition.y += height / 23;
        folders.push_back(Folder(entry.path(), textPosition, currentPath));
        if (textPosition.y <= height - PANEL_OFFSET) {
            lastToDisplay++;
        }
    }
    lastToDisplay--;
}