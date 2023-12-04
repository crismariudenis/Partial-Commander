#include "Folder.h"

void Folder::initText() {

    folderText.setFont(fonts[0]);
    //folderText.setStyle(1);
    folderText.setPosition(position);
    folderText.setFillColor(textColor);
    folderText.setCharacterSize(CHARACTER_SIZE);

    sizeText = folderText;

    std::string canonicPath = this->normalizePath(folderPath.string());
    folderText.setString(canonicPath);

    if (canonicPath != "/..")
        sizeText.setString(toString(size));


    sf::Vector2f sizePosition = position;
    sizePosition.x += FOLDER_SPACE - PANEL_OFFSET;

    sizeText.setPosition(sizePosition);
}

std::string Folder::normalizePath(std::string path) {

    if (path == "/..")
        return path;

    std::string newPath, prefix = parentPath.string();
    int index2 = 0;

    for (int index = 0; index < path.size(); ++index) {

        if (index2 == prefix.size()) {
            for (int j = index; j < path.size(); ++j) {
                newPath += path[j];
            }
            path.clear();
        }
        else {
            if (prefix[index2] == path[index])
                index2++;
        }
    }
    return newPath;
}

std::string Folder::toString(std::uintmax_t size) {
    std::string number;
    if (size == 0) number += "0";
    while (size) {
        number += size % 10 + '0';
        size /= 10;
    }
    std::reverse(number.begin(), number.end());
    return number;
}

Folder::Folder(std::filesystem::path folderPath, sf::Vector2f position, std::filesystem::path parentPath) {
    this->folderPath = folderPath;
    this->position = position;
    this->parentPath = parentPath;
    size = std::filesystem::file_size(this->folderPath);
    initText();
}

sf::Text Folder::getFolderText() {
    return folderText;
}

sf::Text Folder::getSizeText() {
    return sizeText;
}