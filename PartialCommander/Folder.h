#pragma once

#include <filesystem>
#include <SFML/Graphics.hpp>
#include "Config.h"

class Folder
{
private:

	std::filesystem::path folderPath;
	std::uintmax_t size;
	std::string modifyTime;
	sf::Vector2f position;
	sf::Text folderText, sizeText;
	sf::Font font;
	bool isSelected = false;

	std::string normalizePath(std::string path);
	std::string toString(std::uintmax_t size);


public:
	~Folder() {};
	Folder(std::filesystem::path folderPath, sf::Vector2f position, std::vector<sf::Font>& fonts);
	sf::Text getFolderText();
	sf::Text getSizeText();

	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f position);
	void initText(std::vector<sf::Font>& fonts);
	void toggleIsSelected();
	void updateText();
	std::filesystem::path getFolderPath();
};