#include "Scrollbar.h"


void Scrollbar::init(float width, float height, float unitPerFolder) {
	this->setPosition(sf::Vector2f(SCROLLBAR_X, SCROLLBAR_Y));
	this->setSize(sf::Vector2f(width, height));
	this->unitPerFolder = unitPerFolder;
}

void Scrollbar::move(int moveX, int moveY)
{
	sf::Vector2f currPosition = this->getPosition();
	this->setPosition(sf::Vector2f(currPosition.x + moveX, currPosition.y + moveY * unitPerFolder));
}

float Scrollbar::getUnitPerFolder() {
	return unitPerFolder;
}