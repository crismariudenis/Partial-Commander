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

	sf::FloatRect rc = sizeText.getLocalBounds();
	sf::Vector2f sizePosition = position, datePosition = position;
	sizePosition.x += FOLDER_SPACE + SIZE_SPACE + 145;
	dateText = sizeText;
	sizeText.setPosition(sizePosition);
	sizeText.setOrigin(rc.width, 0);

	datePosition.x += FOLDER_SPACE + SIZE_SPACE + 154;
	dateText.setPosition(datePosition);
	dateText.setString(date);
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

Folder::Folder(std::filesystem::path path, sf::Vector2f position, std::vector<sf::Font>& fonts, std::string date) {
	this->path = path;
	this->position = position;
	this->date = date;
	size = std::filesystem::file_size(this->path);
	initText(fonts);	
	std::filesystem::file_time_type ftime1 = std::filesystem::last_write_time(this->path);
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
		dateText.setFillColor(selectedTextColor);
	}
	else
	{
		folderText.setFillColor(sf::Color::White);
		sizeText.setFillColor(sf::Color::White);
		dateText.setFillColor(sf::Color::White);
	}

	sf::Vector2f sizePosition = position, datePosition = position;
	sizePosition.x += FOLDER_SPACE + SIZE_SPACE + 145;
	datePosition.x += FOLDER_SPACE + SIZE_SPACE + 155;
	sizeText.setPosition(sizePosition);
	dateText.setPosition(datePosition);
}


unsigned int Folder::getSize() {
	return size;
}

