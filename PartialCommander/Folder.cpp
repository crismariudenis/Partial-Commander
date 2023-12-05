#include "Folder.h"

void Folder::initText(std::vector<sf::Font>& fonts) {

	folderText.setFont(fonts[3]);
	folderText.setPosition(position);
	folderText.setStyle(sf::Text::Style::Bold);

	folderText.setCharacterSize(CHARACTER_SIZE);

	sizeText = folderText;

	std::string canonicPath = this->normalizePath(folderPath.string());
	folderText.setString(canonicPath);

	if (canonicPath != "/..")
		sizeText.setString(toString(size));

	sf::Vector2f sizePosition = position;
	sizePosition.x += FOLDER_SPACE + SIZE_SPACE / 2 + 120;
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
	if (size == 0)
		number += "6904";
	while (size && number.size() <= 5) {
		number += size % 10 + '0';
		size /= 10;
	}
	std::reverse(number.begin(), number.end());
	return number;
}

Folder::Folder(std::filesystem::path folderPath, sf::Vector2f position, std::filesystem::path parentPath, std::vector<sf::Font>& fonts) {
	this->folderPath = folderPath;
	this->position = position;
	this->parentPath = parentPath;
	size = std::filesystem::file_size(this->folderPath);
	initText(fonts);
}

sf::Text Folder::getFolderText() {
	return folderText;
}

sf::Text Folder::getSizeText() {
	return sizeText;
}

sf::Vector2f Folder::getPosition() {
	return position;
}

void Folder::toggleIsSelected() {
	isSelected = !isSelected;
}

void Folder::setPosition(sf::Vector2f position) {
	this->position = position;
}

void Folder::updateText() {
	folderText.setPosition(position);
	if (isSelected)
	{
		folderText.setFillColor(selectedTextColor);
		sizeText.setFillColor(selectedTextColor);
	}
	else
	{
		folderText.setFillColor(sf::Color::White);
		sizeText.setFillColor(sf::Color::White);
	}

	sf::Vector2f sizePosition = position;
	sizePosition.x += FOLDER_SPACE + SIZE_SPACE / 2 + 120;
	sizeText.setPosition(sizePosition);
}

std::filesystem::path Folder::getFolderPath() {
	return folderPath;
}

std::filesystem::path Folder::getParentPath() {
	return parentPath;
}