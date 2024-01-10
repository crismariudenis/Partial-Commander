#include "Button.h"


Button::Button(std::string text, int height, int width, int index, sf::Vector2f topLeft, sf::Color buttonColor, sf::RenderWindow& window, std::vector<sf::Font>& fonts) : window(window), fonts(fonts)
{
	this->index = index;
	this->text = text;
	this->height = height;
	this->width = width;
	this->topLeft = topLeft;
	this->buttonColor = buttonColor;
}

void Button::draw()
{
	drawRect();
	drawTitle();
	drawIndex();
}

void Button::drawRect() {
	std::string indexString = toString(index);
	sf::RectangleShape rect;
	rect.setFillColor(buttonColor);
	rect.setSize(sf::Vector2f(width - indexString.size() * PANEL_MARGIN_X, height));
	rect.setPosition(sf::Vector2f(topLeft.x + indexString.size() * PANEL_MARGIN_X, topLeft.y));
	window.draw(rect);
}

void Button::drawTitle() {
	std::string indexString = toString(index);
	sf::Text title;
	title.setString(text);
	title.setCharacterSize(CHARACTER_SIZE);
	title.setFont(fonts[3]);
	title.setPosition(sf::Vector2f(topLeft.x + indexString.size() * PANEL_MARGIN_X, topLeft.y));
	title.setFillColor(selectedTextColor);
	window.draw(title);
}

void Button::drawIndex()
{
	sf::Text number;
	std::string indexString = toString(index);
	number.setString(indexString);
	number.setCharacterSize(CHARACTER_SIZE);
	number.setFont(fonts[3]);
	number.setPosition(topLeft);
	number.setFillColor(sf::Color(255, 255, 255));

	window.draw(number);
}

std::string Button::toString(int number) {
	std::string stringified;
	while (number) {
		stringified += (number % 10 + '0');
		number /= 10;
	}
	reverse(stringified.begin(), stringified.end());
	return stringified;
}

void Button::setColor(sf::Color color) {
	buttonColor = color;
}