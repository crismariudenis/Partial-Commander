#pragma once

#include "config.h"

class Button
{
private:
	std::string text;
	int height, width, index;
	sf::Vector2f topLeft;
	sf::Color buttonColor;
	sf::RenderWindow& window;
	std::vector<sf::Font>& fonts;

	void drawRect();
	void drawTitle();
	void drawIndex();
	std::string toString(int number);

public:
	Button(std::string text ,int height, int width, int index, sf::Vector2f topLeft, sf::Color buttonColor, sf::RenderWindow & window, std::vector<sf::Font> &fonts);
	void draw();
};