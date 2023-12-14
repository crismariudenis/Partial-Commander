#pragma once

#include "Panel.h"
#include "Button.h"

class App
{
private:
	sf::RectangleShape background, bottomBackground;
	sf::RenderWindow mainWindow;
	std::string name;
	Panel leftPanel{ mainWindow }, rightPanel{ mainWindow };
	std::vector<sf::Font> fonts = fontsHandler.getFonts();
	std::vector<Button> buttons;

	void initWindow(std::string name);
	void initPanels();
	void initButtons();
	void handleKeyboardEvents(sf::Event& event);
	void handleMouseScrollingEvents(sf::Event& event);
	void handleMousePressingEvents(sf::Event & event);
	void handleMouseMovedEvents();

	void drawButtons();
	void initBackgrounds();

public:
	void init();
	void run();
	App(std::string name) :name(name) {};

};