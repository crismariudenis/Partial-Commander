#include "Folder.h"

void Folder::initText(std::vector<sf::Font>& fonts) {

	folderText.setFont(fonts[CustomFonts::Font::ROBOTO]);
	folderText.setPosition(position);

	folderText.setCharacterSize(CHARACTER_SIZE);

	sizeText = folderText;

	std::string canonicPath = this->normalizePath(path.string());
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

	std::string newPath, prefix = (this->path).parent_path().string();
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

Folder::Folder(std::filesystem::path path, sf::Vector2f position, std::vector<sf::Font>& fonts) {
	this->path = path;
	this->position = position;
	size = std::filesystem::file_size(this->path);
	initText(fonts);
}

void Folder::toggleIsSelected() {
	isSelected = !isSelected;
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

