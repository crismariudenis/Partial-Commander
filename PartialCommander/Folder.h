#pragma once

#include <filesystem>
#include <SFML/Graphics.hpp>
#include "Config.h"

class Folder
{
private:

	std::uintmax_t size;
	std::string modifyTime;
	sf::Font font;
	bool isSelected = false;

	std::string normalizePath(std::string path);
	std::string toString(std::uintmax_t size);


public:
	~Folder() {};
	Folder(std::filesystem::path path, sf::Vector2f position, std::vector<sf::Font>& fonts);
	void initText(std::vector<sf::Font>& fonts);
	void toggleIsSelected();
	void updateText();
	std::filesystem::path path;
	sf::Text folderText, sizeText;
	sf::Vector2f position;
};