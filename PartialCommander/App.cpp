#include "App.h"

void App::initWindow(std::string name) {

	mainWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), name);

	/// Calculating topLeft corner so that window is in the middle of the screen
	sf::VideoMode mode = sf::VideoMode::getDesktopMode();
	sf::Vector2i topLeft(mode.width / 10, mode.height / 10);

	mainWindow.setPosition(topLeft);
}


void App::initBackgrounds() {
	initBackground(background, backgroundColor, sf::Vector2f(0, 0), sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	initBackground(bottomBackground, bottomBackgroundColor, sf::Vector2f(0, WINDOW_HEIGHT - BOTTOM_BUTTONS_HEIGHT + 22),
		sf::Vector2f(WINDOW_WIDTH, BOTTOM_BUTTONS_HEIGHT));
}

void App::init() {
	initWindow(name);
	initBackgrounds();
	initPanels();
	initButtons();
}

void App::run() {

	while (mainWindow.isOpen()) {

		/// Event handling
		sf::Event event;
		while (mainWindow.pollEvent(event)) {
			switch (event.type)
			{
			case sf::Event::Closed:
				mainWindow.close();
				break;
			case sf::Event::KeyPressed:
				handleKeyboardEvents(event);
				break;
			case sf::Event::MouseWheelScrolled:
				handleMouseScrollingEvents(event);
				break;
			case sf::Event::MouseButtonPressed:
				handleMousePressingEvents(event);
				break;

			}
		}

		mainWindow.setActive();

		/*  Drawing operations */

		/// Background colors
		mainWindow.draw(background);
		mainWindow.draw(bottomBackground);
		handleMouseMovedEvents();
		/// Panels
		leftPanel.draw();
		rightPanel.draw();
		

		/// Buttons
		drawButtons();

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
	sf::Vector2f left{ PANEL_MARGIN_X,PANEL_MARGIN_TOP + TOP_BUTTONS_HEIGHT};
	sf::Vector2f right{ 2 * PANEL_MARGIN_X + PANEL_WIDTH - 3 * PANEL_LINE_WIDTH + left.x,left.y };

	leftPanel.init(left, PANEL_WIDTH, PANEL_HEIGHT, leftPanelDirectoryPath, fonts);
	rightPanel.init(right, PANEL_WIDTH, PANEL_HEIGHT, rightPanelDirectoryPath, fonts);

	leftPanel.toggleIsSelected();
}

void App::handleKeyboardEvents(sf::Event& event) {
	switch (event.key.scancode)
	{
	case sf::Keyboard::Scan::S: case sf::Keyboard::Scan::W: case sf::Keyboard::Scan::Down: case sf::Keyboard::Scan::Up:
		leftPanel.updateSelectedFolder(event.key.scancode);
		rightPanel.updateSelectedFolder(event.key.scancode);
		break;
	case sf::Keyboard::Scan::Tab:
		leftPanel.toggleIsSelected();
		rightPanel.toggleIsSelected();
		break;
	case sf::Keyboard::Scan::Enter:
		leftPanel.changePath();
		rightPanel.changePath();
		break;
	case sf::Keyboard::Scancode::F8:
		leftPanel.updateSelectedFolder(event.key.scancode);
		rightPanel.updateSelectedFolder(event.key.scancode);
		break;
	case sf::Keyboard::Scancode::F5:
		leftPanel.updateSelectedFolder(event.key.scancode);
		rightPanel.updateSelectedFolder(event.key.scancode);
		break;
	}
}

void App::handleMousePressingEvents(sf::Event &event) 
{
	if (event.mouseButton.button == sf::Mouse::Left) {
		int mouseX = event.mouseButton.x;
		int mouseY = event.mouseButton.y;
		leftPanel.checkTextLabels(mouseX, mouseY);
		rightPanel.checkTextLabels(mouseX, mouseY);
	}
}

void App::handleMouseMovedEvents() {
	sf::Vector2i position = sf::Mouse::getPosition(mainWindow);
	int mouseX = position.x, mouseY = position.y;
	leftPanel.activateLabel(mouseX, mouseY);
	rightPanel.activateLabel(mouseX, mouseY);
}

void App::handleMouseScrollingEvents(sf::Event& event)
{
	int delta = event.mouseWheelScroll.delta;
	if (delta < 0) {
		leftPanel.updateSelectedFolder(sf::Keyboard::Scan::S);
		rightPanel.updateSelectedFolder(sf::Keyboard::Scan::S);
	}
	else {
		leftPanel.updateSelectedFolder(sf::Keyboard::Scan::W);
		rightPanel.updateSelectedFolder(sf::Keyboard::Scan::W);
	}
}

void App::initButtons() {

	sf::Vector2f topLeft(PANEL_MARGIN_X * 2, WINDOW_HEIGHT - PANEL_BOTTOM_HEIGHT / 1.25);
	int moveX = (WINDOW_WIDTH - PANEL_MARGIN_X * (buttonNames.size() + 2)) / buttonNames.size();

	for (int index = 0;index < buttonNames.size(); ++index) {
		Button button(buttonNames[index], BUTTON_HEIGHT, moveX, index + 1, topLeft, secondaryColor, mainWindow, fonts);
		buttons.push_back(button);
		topLeft += sf::Vector2f(moveX + PANEL_MARGIN_X, 0);
	}
}

void App::drawButtons() {
	for (int index = 0; index < buttons.size(); ++index)
		buttons[index].draw();
}