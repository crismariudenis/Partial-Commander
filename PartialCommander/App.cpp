#include "App.h"

void App::initWindow(std::string name) {

	mainWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), name);

	/// Calculating topLeft corner so that window is in the middle of the screen
	sf::VideoMode mode = sf::VideoMode::getDesktopMode();
	sf::Vector2i topLeft(mode.width / 10, mode.height / 10);

	mainWindow.setPosition(topLeft);
}

void App::initBackgroundColor(sf::RectangleShape& background) {

	background.setFillColor(backgroundColor);
	background.setPosition(0, 0);
	background.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
}

void App::init() {
	initWindow(name);
	initBackgroundColor(background);
	initPanels();
}

App::App(std::string name) {
	this->name = name;
}

void App::run() {
	
	while (mainWindow.isOpen()) {

		/// Event handling
		sf::Event event;
		while (mainWindow.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				mainWindow.close();
		}

		mainWindow.setActive();

		/// Drawing operations
		mainWindow.draw(background);
		leftPanel.draw();
		rightPanel.draw();

		mainWindow.display();

	}
}

void App::initPanels() {
	/*
					  margin_top
	 margin_x [left] 2 * margin_x [right] margin_x
					margin_bottom
					margin_bottom
	*/
	sf::Vector2f left{ PANEL_MARGIN_X,PANEL_MARGIN_TOP };
	sf::Vector2f right{ 2 * PANEL_MARGIN_X + PANEL_WIDTH - 3 * PANEL_LINE_WIDTH + left.x,left.y };
	
	leftPanel.init(left, PANEL_WIDTH, PANEL_HEIGHT, leftPanelDirectoryPath, fonts);
	rightPanel.init(right, PANEL_WIDTH, PANEL_HEIGHT, rightPanelDirectoryPath, fonts);
}