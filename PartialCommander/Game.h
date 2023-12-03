#pragma once

#include "Panel.h"

class Game
{
private:
	sf::RectangleShape background;
	sf::RenderWindow mainWindow;
	std::string name;
	Panel leftPanel, rightPanel;

	void initWindow(sf::RenderWindow& mainWindow, std::string name);
	void initBackgroundColor(sf::RectangleShape & shape);
	void initPanels();

public:
	void init();
	void run();
	Game(std::string name);

};