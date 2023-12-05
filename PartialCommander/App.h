#pragma once

#include "Panel.h"

class App
{
private:
	sf::RectangleShape background;
	sf::RenderWindow mainWindow;
	std::string name;
	Panel leftPanel{ mainWindow }, rightPanel{ mainWindow };
	std::vector<sf::Font> fonts = fontsHandler.getFonts();

	void initWindow(std::string name);
	void initBackgroundColor(sf::RectangleShape& shape);
	void initPanels();

public:
	void init();
	void run();
	App(std::string name);

};