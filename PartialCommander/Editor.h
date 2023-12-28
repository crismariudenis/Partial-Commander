#pragma once

#include "Config.h"
#include "System.h"

class Editor {
	sf::RenderWindow& window;
public:
	static enum Mode {
		EDIT,
		VIEW
	};
	Mode mode;
	Editor(sf::RenderWindow& window, Mode mode) :window(window), mode(mode) {};
	void update(sf::Event event);
	void draw();
};