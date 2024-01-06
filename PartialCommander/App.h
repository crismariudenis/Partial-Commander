#pragma once


#include "Button.h"
#include "Editor.h"
#include "Clipboard.h"
#include "Scrollbar.h"

class App
{
private:
	System* sys = System::getInstance();
	Clipboard* clipboard = Clipboard::getInstance();
	int timer = 0, count = 0;
	sf::RectangleShape background, bottomBackground, upButton, downButton;
	sf::RenderWindow window;
	sf::Text buttonText, buttonText2;
	std::string name;
	Panel leftPanel{ window }, rightPanel{ window };
	Scrollbar scrollbar;
	Editor* editor;
	std::vector<sf::Font> fonts = fontsHandler.getFonts();
	std::vector<Button> buttons;
	std::map<std::pair<int, int>, bool> mouseClicked;
	
	bool renameShortcut = false;
	std::string renameString;

	void handleKeyboardEvents(sf::Event& event);
	void handleMouseScrollingEvents(sf::Event& event, Panel &panel);
	void handleMousePressingEvents(sf::Event& event, Panel &panel);
	void handleMouseMovedEvents(Panel & panel);
	void handleKeyboardShortcuts(sf::Event event, Panel &panel);
	void handleRenameShortcut(sf::Event event, Panel& panel);

	void drawButtons();

	void initScrollbarButtons();
	void initWindow(std::string name);
	void initPanels();
	void initButtons();

	void getCursor(sf::Cursor& cursor, Panel & panel);

public:

	bool isMouseOnScrollbar = false, shortcutOn = false, pathShortcut = false;

	void init();
	void run();
	App(std::string name) :name(name) {};

};