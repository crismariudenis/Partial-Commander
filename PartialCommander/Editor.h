#pragma once

#include "Config.h"
#include "System.h"

class Editor {
	sf::RenderWindow& window;
	std::filesystem::path path;
	std::vector<std::string>text;
	int firstLine = 0, lastLine = 0;
public:
	static enum Mode {
		EDIT,
		VIEW
	};
	Mode mode;
	Editor(sf::RenderWindow& window, Mode mode) :window(window), mode(mode) {};
	void init(std::filesystem::path path);
	void update(sf::Event event);
	void draw();
};