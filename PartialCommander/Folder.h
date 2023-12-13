#pragma once

#include <filesystem>
#include <SFML/Graphics.hpp>
#include "Config.h"

class Folder
{
private:

	std::uintmax_t size;
	sf::Font font;
	std::string modifyTime;
	bool isSelected = false;

	std::string normalizePath(std::string path);
	std::string toString(std::uintmax_t size);


public:
	~Folder() {};
	Folder(std::filesystem::path path, sf::Vector2f position, std::vector<sf::Font>& fonts, std::string date);
	void initText(std::vector<sf::Font>& fonts);
	void toggleIsSelected();
	void updateText();
	std::filesystem::path path;
	sf::Text folderText, sizeText, dateText;
	sf::Vector2f position;
	std::string date;
};