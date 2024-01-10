#pragma once

#include "Config.h"
#include "System.h"
#include "Button.h"

class Editor {
	struct TextLine {
		std::string string;
		sf::Text text;
	};
	sf::RenderWindow& window;
	std::filesystem::path path;
	std::vector<TextLine>lines;
	std::vector<sf::Font>fonts;
	int firstLine = 0, lastLine = 0;
	sf::Vector2i cursorPos{ 0,0 };
	std::vector<Button>buttons;
	int frames = 0; // mod 120
	void edit(sf::Event event);
	void repairScrolling();
	void save();
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