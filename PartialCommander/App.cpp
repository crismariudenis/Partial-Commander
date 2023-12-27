#include "App.h"

void App::initWindow(std::string name) {

	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), name);

	/// Calculating topLeft corner so that window is in the middle of the screen
	sf::VideoMode mode = sf::VideoMode::getDesktopMode();
	sf::Vector2i topLeft(mode.width / 10, mode.height / 10);

	window.setPosition(topLeft);
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

	while (window.isOpen()) {

		/// Event handling
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::KeyPressed: case sf::Event::KeyReleased:
				handleKeyboardShortcuts(event);
				handleKeyboardEvents(event);
				break;
			case sf::Event::MouseWheelScrolled:
				handleMouseScrollingEvents(event);
				break;
			case sf::Event::MouseButtonPressed:
				handleMousePressingEvents(event);
				break;
			case sf::Event::MouseButtonReleased:
				isMouseOnScrollbar = false;
				break;

			}
		}

		window.setActive();

		sf::Cursor cursor;
		getCursor(cursor);
		window.setMouseCursor(cursor);

		/*  Drawing operations */

		/// Background colors
		window.draw(background);
		window.draw(bottomBackground);
		handleMouseMovedEvents();
		/// Panels
		leftPanel.draw();
		rightPanel.draw();


		/// Buttons
		drawButtons();

		window.display();
	}
}

void App::initPanels() {
	/*
					  margin_top
	 margin_x [left] 2 * margin_x [right] margin_x
					margin_bottom
					margin_bottom
	*/
	sf::Vector2f left{ PANEL_MARGIN_X,PANEL_MARGIN_TOP + TOP_BUTTONS_HEIGHT };
	sf::Vector2f right{ 2 * PANEL_MARGIN_X + PANEL_WIDTH - 3 * PANEL_LINE_WIDTH + left.x,left.y };

	leftPanel.init(left, PANEL_WIDTH, PANEL_HEIGHT, leftPanelDirectoryPath, fonts);
	rightPanel.init(right, PANEL_WIDTH, PANEL_HEIGHT, rightPanelDirectoryPath, fonts);

	leftPanel.toggleIsSelected();
}

void App::handleKeyboardEvents(sf::Event& event) {
	if (event.type == sf::Event::KeyReleased)
		return;
	switch (event.key.scancode)
	{
	case sf::Keyboard::Scan::S: case sf::Keyboard::Scan::W: case sf::Keyboard::Scan::Down: case sf::Keyboard::Scan::Up:
		if (!shortcutOn) {
			leftPanel.updateSelectedFolder(event.key.scancode);
			rightPanel.updateSelectedFolder(event.key.scancode);
		}
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
	case sf::Keyboard::Scancode::F10:
		window.close();
		break;
	}
}

void App::handleKeyboardShortcuts(sf::Event event)
{
	bool keyPressed = false;
	if (event.type == sf::Event::KeyReleased) {
		released[event.key.scancode] = true;
		pressed[event.key.scancode] = false;
	}
	else if (event.type == sf::Event::KeyPressed) {
		keyPressed = true;
		released[event.key.scancode] = false;
		pressed[event.key.scancode] = true;
	}

	if (pressed[sf::Keyboard::Scan::LControl] && pressed[sf::Keyboard::Scan::A]) {
		shortcutOn = true;
		leftPanel.updateShortcutSelectedFolder(1, 0);
		rightPanel.updateShortcutSelectedFolder(1, 0);
	}
	else if (pressed[sf::Keyboard::Scan::LControl] && pressed[sf::Keyboard::Scan::LShift]) {
		shortcutOn = true;
		if (pressed[sf::Keyboard::Scan::Up] || pressed[sf::Keyboard::Scan::Down]) {
			int move = 1;
			if (pressed[sf::Keyboard::Scan::Up]) move = -move;
			leftPanel.updateShortcutSelectedFolder(2, move);
			rightPanel.updateShortcutSelectedFolder(2, move);
			leftPanel.updateByScrollbar(move);
			rightPanel.updateByScrollbar(move);
		}
	}
	else if (pressed[sf::Keyboard::Scan::LControl] && pressed[sf::Keyboard::Scan::C]) {
		shortcutOn = true;
		leftPanel.updateClipboard();
		rightPanel.updateClipboard();
	}
	else if (pressed[sf::Keyboard::Scan::LControl] && pressed[sf::Keyboard::Scan::V]) {
		shortcutOn = true;
		Clipboard* clipboard = Clipboard::getInstance();
		leftPanel.pasteFromClipboard(clipboard->getFolders());
		rightPanel.pasteFromClipboard(clipboard->getFolders());
	}
	else if (pressed[sf::Keyboard::Scan::Semicolon]) {
		int keysPressed = 0;
		sf::Keyboard::Scancode code;
		for (auto el : pressed) {
			if (el.second == true)
				keysPressed++;
			if (el.first != sf::Keyboard::Scan::Semicolon && el.second == true) {
				code = el.first;
			}
		}////
		if (keysPressed == 2 && code >= 0 && code <= 25) {
			std::string path;
			path += (code + 'A');
			path += ":\\";
			std::filesystem::path directoryPath(path);
			leftPanel.changeDirectory(directoryPath);
			rightPanel.changeDirectory(directoryPath);

		}
	}
	else if(shortcutOn == true && !pressed[sf::Keyboard::Scan::LControl] && keyPressed == true && !pressed[sf::Keyboard::Scan::LShift] && event.key.scancode != sf::Keyboard::Scan::Space) {
		leftPanel.updateShortcutSelectedFolder(3, -1);
		rightPanel.updateShortcutSelectedFolder(3, -1);
		shortcutOn = false;
	}

	if (pressed[sf::Keyboard::Scan::LControl] && pressed[sf::Keyboard::Scan::F]) {
		leftPanel.activateSearch();
		rightPanel.activateSearch();
	}
	else if (keyPressed) {
		leftPanel.registerCharacter(event.key.scancode, pressed[sf::Keyboard::Scan::LShift]);
		rightPanel.registerCharacter(event.key.scancode, pressed[sf::Keyboard::Scan::LShift]);
	}

	if (pressed[sf::Keyboard::Scan::Space]) {
		leftPanel.deactivateSearch();
		rightPanel.deactivateSearch();
	}
	
}

void App::handleMousePressingEvents(sf::Event& event)
{
	if (event.mouseButton.button == sf::Mouse::Left) {

		sf::Vector2f mouse{ (float)event.mouseButton.x , (float)event.mouseButton.y };
		if (pressed[sf::Keyboard::Scan::LControl]) {
				float height = PANEL_HEIGHT;
				leftPanel.updateShortcutSelectedFolder(4, (int)((mouse.y - 108.261) / (height / LINE_SPACING)));
				rightPanel.updateShortcutSelectedFolder(4, (int)((mouse.y - 108.261) / (height / LINE_SPACING)));
				shortcutOn = true;
				}
		if (!shortcutOn) {
			leftPanel.checkTextLabels(mouse);
			rightPanel.checkTextLabels(mouse);
			leftPanel.checkFolderLabels(mouse);
			rightPanel.checkFolderLabels(mouse);
		}
		bool ok = leftPanel.checkScrollbarLabel(mouse);
		if (ok)
			isMouseOnScrollbar = true;
		ok = rightPanel.checkScrollbarLabel(mouse);
		if (ok)
			isMouseOnScrollbar = true;

		auto checkScrollbarButton = [&](sf::RectangleShape & button)
			{
				sf::Vector2f buttonPosition = button.getPosition();
				sf::Vector2f buttonSize = button.getSize();
				return mouse.x >= buttonPosition.x && mouse.x <= buttonPosition.x + buttonSize.x && mouse.y >= buttonPosition.y && mouse.y <= buttonPosition.y + buttonSize.y;
			};

		if (checkScrollbarButton(upButton))
			leftPanel.updateByScrollbar(-1), rightPanel.updateByScrollbar(-1);
		else if (checkScrollbarButton(downButton))
			leftPanel.updateByScrollbar(1), rightPanel.updateByScrollbar(1);
	}
}

void App::handleMouseMovedEvents() {
	sf::Vector2i position = sf::Mouse::getPosition(window);
	int mouseX = position.x, mouseY = position.y;
	leftPanel.activateLabel(mouseX, mouseY);
	rightPanel.activateLabel(mouseX, mouseY);
	if (isMouseOnScrollbar) {///
		leftPanel.updateByScrollbar(sf::Vector2f((float)mouseX, (float)mouseY));
		rightPanel.updateByScrollbar(sf::Vector2f((float)mouseX, (float)mouseY));
	}
}

void App::handleMouseScrollingEvents(sf::Event& event)
{	
	int delta = static_cast<int>(event.mouseWheelScroll.delta);
	if (delta < 0) {
		leftPanel.updateSelectedFolder(sf::Keyboard::Scan::S);
		rightPanel.updateSelectedFolder(sf::Keyboard::Scan::S);
	}
	else {
		leftPanel.updateSelectedFolder(sf::Keyboard::Scan::W);
		rightPanel.updateSelectedFolder(sf::Keyboard::Scan::W);
	}
	leftPanel.updateShortcutSelectedFolder(3, -1);
	rightPanel.updateShortcutSelectedFolder(3, -1);
	shortcutOn = false;
}

void App::initButtons() {

	sf::Vector2f topLeft(PANEL_MARGIN_X * 2, WINDOW_HEIGHT - PANEL_BOTTOM_HEIGHT / 1.25);
	int moveX = (WINDOW_WIDTH - PANEL_MARGIN_X * (buttonNames.size() + 2)) / buttonNames.size();

	for (unsigned int index = 0; index < buttonNames.size(); ++index) {
		Button button(buttonNames[index], BUTTON_HEIGHT, moveX, index + 1, topLeft, secondaryColor, window, fonts);
		buttons.push_back(button);
		topLeft += sf::Vector2f(moveX * 1.f + PANEL_MARGIN_X, 0);
	}
}

void App::drawScrollbarButtons()
{
	sf::Text buttonText, buttonText2;
	auto initScrollBarButton = [](sf::RectangleShape& button, sf::Vector2f position)
		{
			button.setFillColor(scrollbarButtonColor);
			button.setSize(sf::Vector2f(SCROLLBAR_BUTTON_WIDTH, SCROLLBAR_BUTTON_HEIGHT));
			button.setPosition(position);
			button.setOutlineThickness(2);
		};
	auto initScrollBarButtonText = [&](sf::Text& buttonText, unsigned int characterSize, std::string text, sf::Vector2f position)
		{
			buttonText.setFillColor(scrollbarTextButtonColor);
			buttonText.setFont(fonts[CustomFonts::Font::ROBOTO]);
			buttonText.setString(text);
			buttonText.setPosition(position);
			buttonText.setCharacterSize(characterSize);
		};

	initScrollBarButton(upButton, sf::Vector2f(SCROLLBAR_X, SCROLLBAR_Y + 2));
	initScrollBarButton(downButton, sf::Vector2f(PANEL_WIDTH + PANEL_MARGIN_X + 2, PANEL_HEIGHT + TOP_BUTTONS_HEIGHT));
	initScrollBarButtonText(buttonText, 24, "^", sf::Vector2f(SCROLLBAR_X, SCROLLBAR_Y));
	initScrollBarButtonText(buttonText2, 18, "v", sf::Vector2f(downButton.getPosition().x, downButton.getPosition().y));

	window.draw(upButton), window.draw(downButton);
	window.draw(buttonText), window.draw(buttonText2);
}

void App::drawButtons() {
	for (unsigned int index = 0; index < buttons.size(); ++index)
		buttons[index].draw();
	drawScrollbarButtons();
}

void App::getCursor(sf::Cursor &cursor) {
	cursor.loadFromSystem(sf::Cursor::Hand);
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	if (mousePosition.y >= 0 && mousePosition.y <= TOP_BUTTONS_HEIGHT + 15)
		cursor.loadFromSystem(sf::Cursor::Arrow);
	if (mousePosition.y >= TOP_BUTTONS_HEIGHT + PANEL_HEIGHT - BOTTOM_BUTTONS_HEIGHT + 15 && mousePosition.y <= window.getSize().y)
		cursor.loadFromSystem(sf::Cursor::Arrow);
	if (leftPanel.checkMouseOnFolder(leftPanel.getSelectedFolderIndex(), (float)mousePosition.x, (float)mousePosition.y) || rightPanel.checkMouseOnFolder(rightPanel.getSelectedFolderIndex(), (float)mousePosition.x, (float)mousePosition.y))
		cursor.loadFromSystem(sf::Cursor::Arrow);
}