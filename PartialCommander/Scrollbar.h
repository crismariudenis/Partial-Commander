#pragma once

#include "Config.h"


class Scrollbar : public sf::RectangleShape {
	
	float unitPerFolder;

public:
	void init(float width, float height, float unitPerFolder);
	Scrollbar() {};
	void move(int moveX, int moveY);
	float getUnitPerFolder();
	void update(int steps);
	void update(sf::Vector2f mouse);
};