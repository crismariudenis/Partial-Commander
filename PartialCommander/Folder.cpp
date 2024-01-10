#include "Folder.h"

void Folder::initText(std::vector<sf::Font>& fonts) {

	folderText.setFont(fonts[CustomFonts::Font::UBUNTU]);
	folderText.setPosition(position);
	folderText.setCharacterSize(CHARACTER_SIZE);

	if (isSelected)
		folderText.setFillColor(selectedTextColor);

	sizeText = folderText;

	std::string canonicPath = this->normalizePath(path.string());
	if (canonicPath.size() > 50)
		canonicPath = "..." + canonicPath.substr(canonicPath.size() - 50, canonicPath.size());
	folderText.setString(canonicPath);

	if (canonicPath != "/..")
		sizeText.setString(toString(size));

	sf::Vector2f sizePosition = position, datePosition = position;

	sf::FloatRect rc = sizeText.getLocalBounds();
	sizePosition.x += FOLDER_SPACE + SIZE_SPACE - 15;
	dateText = sizeText;
	sizeText.setPosition(sizePosition);
	sizeText.setOrigin(rc.width, 0);

	dateText.setString(date);
	sf::FloatRect rc2 = dateText.getLocalBounds();
	datePosition.x += FOLDER_SPACE + SIZE_SPACE + TIME_SPACE - 15;
	dateText.setPosition(datePosition);
	dateText.setOrigin(rc2.width, 0);
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
	if (size == 0 or size == 4096)
		return "<DIR>";
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
	this->date = calculateDate(path);
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
		dateText.setFillColor(selectedTextColor);
	}
	else
	{
		folderText.setFillColor(textColor);
		sizeText.setFillColor(textColor);
		dateText.setFillColor(textColor);
	}
	sf::Vector2f sizePosition = position, datePosition = position;

	sf::FloatRect rc = sizeText.getLocalBounds();
	sizePosition.x += FOLDER_SPACE + SIZE_SPACE - 15;
	dateText = sizeText;
	sizeText.setPosition(sizePosition);
	sizeText.setOrigin(rc.width, 0);

	dateText.setString(date);
	sf::FloatRect rc2 = dateText.getLocalBounds();
	datePosition.x += FOLDER_SPACE + SIZE_SPACE + TIME_SPACE - 15;
	dateText.setPosition(datePosition);
	dateText.setOrigin(rc2.width, 0);
}


unsigned int Folder::getSize() const {
	return size;
}


std::string Folder::calculateDate(std::filesystem::path path) {
	// Convert from fs::file_time_type to std::time_t
	auto to_time_t = [](auto tp) {
		namespace cs = std::chrono;
		namespace fs = std::filesystem;
		auto sctp = cs::time_point_cast<cs::system_clock::duration>(tp -
			fs::file_time_type::clock::now() + cs::system_clock::now());
		return cs::system_clock::to_time_t(sctp);
		};


	auto fileTime = std::filesystem::last_write_time(path);
	std::time_t tt = to_time_t(fileTime);

	// Convert the std::time_t value to a local time structure
	std::tm* localTime = std::localtime(&tt);
	std::stringstream buff;

	buff << std::put_time(localTime, "%d %b %H:%M");
	std::string formattedDate = buff.str();
	return formattedDate;
}