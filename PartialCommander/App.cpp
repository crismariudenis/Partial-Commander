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

		timer++;
		if (timer % CLICK_ERROR == 0)
			mouseClicked.clear(), timer = 0;

		Panel& panel = rightPanel.isSelected ? rightPanel : leftPanel;

		/// Event handling
		sf::Event event;
		bool prevRename = renameShortcut;
		while (window.pollEvent(event)) {
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed: case sf::Event::KeyReleased: 
				handleKeyboardShortcuts(event, panel);
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
			if(prevRename)
				handleRenameShortcut(event, panel);
		}


		window.setActive();

		sf::Cursor cursor;
		getCursor(cursor);
		window.setMouseCursor(cursor);

		/*  Drawing operations */

		/// Background colors
		window.draw(background);
		window.draw(bottomBackground);


		if (!editor) {
			handleMouseMovedEvents();
			/// Panels
			leftPanel.draw();
			rightPanel.draw();
			/// Buttons
			drawButtons();
		}
		else
			editor->draw();


		window.display();
	}
}

void App::initPanels() {
	sf::Vector2f left{ PANEL_MARGIN_X,PANEL_MARGIN_TOP + TOP_BUTTONS_HEIGHT };
	sf::Vector2f right{ 2 * PANEL_MARGIN_X + PANEL_WIDTH - 3 * PANEL_LINE_WIDTH + left.x,left.y };

	leftPanel.init(left, PANEL_WIDTH, PANEL_HEIGHT, leftPanelDirectoryPath, fonts);
	rightPanel.init(right, PANEL_WIDTH, PANEL_HEIGHT, rightPanelDirectoryPath, fonts);

	leftPanel.toggleIsSelected();
}

void App::handleKeyboardEvents(sf::Event& event) {
	if (event.type == sf::Event::KeyReleased || renameShortcut)
		return;

	System* system = System::getInstance();

	if (editor) {
		switch (event.key.scancode)
		{
		case sf::Keyboard::Scancode::F10:
			delete editor;
			editor = NULL;
			break;
		default:
			editor->update(event);
		}
		return;
	}
	Panel& panel = rightPanel.isSelected ? rightPanel : leftPanel;
	switch (event.key.scancode)
	{
	 case sf::Keyboard::Scan::Down: case sf::Keyboard::Scan::Up:
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
	case sf::Keyboard::Scancode::F3:
		editor = new Editor(window, Editor::Mode::VIEW);
		editor->init(panel.folders[panel.selectedFolderIndex].path);
		editor->update(event);
		break;
	case sf::Keyboard::Scancode::F4:
		editor = new Editor(window, Editor::Mode::EDIT);
		editor->init(panel.folders[panel.selectedFolderIndex].path);
		editor->update(event);
		break;
	}
}

void App::handleRenameShortcut(sf::Event event, Panel &panel) {
	if (!renameShortcut)
		return;
	if(event.type == sf::Event::KeyPressed)
		panel.registerCharacter(event.key.scancode, pressed[sf::Keyboard::Scan::LShift], 2);
	if (event.key.scancode == sf::Keyboard::Scan::Enter || event.key.scancode == sf::Keyboard::Scan::Up || event.key.scancode == sf::Keyboard::Scan::Down) {
		panel.updateShortcutSelectedFolder(3, -1);
		renameShortcut = false;
		if (event.key.scancode == sf::Keyboard::Scan::Enter)
			panel.rename(renameString);
		else panel.setSelectedFolder(renameString);
	}
	else {
		if (event.type != sf::Event::KeyPressed && event.type != sf::Event::KeyReleased && event.type != sf::Event::TextEntered) {
			panel.updateShortcutSelectedFolder(3, -1);

			renameShortcut = false;
		}
	}
}

void App::handleKeyboardShortcuts(sf::Event event, Panel & panel)
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
		panel.updateShortcutSelectedFolder(1, 0);
	}
	else if (pressed[sf::Keyboard::Scan::LControl] && pressed[sf::Keyboard::Scan::LShift]) {
		shortcutOn = true;
		if (pressed[sf::Keyboard::Scan::Up] || pressed[sf::Keyboard::Scan::Down]) {
			int move = 1;
			if (pressed[sf::Keyboard::Scan::Up]) move = -move;
			panel.updateShortcutSelectedFolder(2, move);
			panel.updateByScrollbar(move);
		}
	}
	else if (pressed[sf::Keyboard::Scan::LControl] && pressed[sf::Keyboard::Scan::C]) {
		shortcutOn = true;
		panel.updateClipboard();
	}
	else if (pressed[sf::Keyboard::Scan::LControl] && pressed[sf::Keyboard::Scan::V]) {
		shortcutOn = true;
		Clipboard* clipboard = Clipboard::getInstance();
		panel.pasteFromClipboard(clipboard->getFolders());
	}
	else if (pressed[sf::Keyboard::Scan::LControl] && pressed[sf::Keyboard::Scan::R]) {
		panel.registerCharacter(255, 0, 2);
		renameShortcut = true;
		renameString = panel.getSelectedFolder().folderText.getString();
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
			panel.changeDirectory(directoryPath);
		}
	}
	else if (shortcutOn == true && !pressed[sf::Keyboard::Scan::LControl] && keyPressed == true && !pressed[sf::Keyboard::Scan::LShift] && event.key.scancode != sf::Keyboard::Scan::Space) {
		if (event.key.scancode != sf::Keyboard::Scan::F5 && event.key.scancode != sf::Keyboard::Scan::F8) {
			panel.updateShortcutSelectedFolder(3, -1);
			shortcutOn = false;
		}
	}

	if (pressed[sf::Keyboard::Scan::LControl] && pressed[sf::Keyboard::Scan::F]) {
		panel.activateSearch();
		renameShortcut = false;
	}
	else if (keyPressed) 
		panel.registerCharacter(event.key.scancode, pressed[sf::Keyboard::Scan::LShift], 1);
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

		auto checkScrollbarButton = [&](sf::RectangleShape& button)
			{
				sf::Vector2f buttonPosition = button.getPosition();
				sf::Vector2f buttonSize = button.getSize();
				return mouse.x >= buttonPosition.x && mouse.x <= buttonPosition.x + buttonSize.x && mouse.y >= buttonPosition.y && mouse.y <= buttonPosition.y + buttonSize.y;
			};

		if (checkScrollbarButton(upButton))
			leftPanel.updateByScrollbar(-1), rightPanel.updateByScrollbar(-1);
		else if (checkScrollbarButton(downButton))
			leftPanel.updateByScrollbar(1), rightPanel.updateByScrollbar(1);
		if (mouseClicked[{mouse.x, mouse.y}] == true) {
			if (mouse.y >= PANEL_MARGIN_TOP + TOP_BUTTONS_HEIGHT + 38.f && mouse.y <= TOP_BUTTONS_HEIGHT + PANEL_HEIGHT + PANEL_MARGIN_TOP  - BOTTOM_BUTTONS_HEIGHT)
			{
				leftPanel.changePath();
				rightPanel.changePath();
			}
		}
		mouseClicked.clear();
		mouseClicked[{mouse.x, mouse.y}] = true;
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
	leftPanel.updateShortcutSelectedFolder(3, -1);
	rightPanel.updateShortcutSelectedFolder(3, -1);
	renameShortcut = false;
	if (editor) {
		editor->update(event);
		return;
	}
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
	auto initScrollBarButtonText = [&](sf::Text& buttonText, unsigned int characterSize, std::string text, sf::Vector2f position, float angle)
		{
			buttonText.setFillColor(scrollbarTextButtonColor);
			buttonText.setFont(fonts[CustomFonts::Font::ROBOTO]);
			buttonText.setString(text);
			buttonText.setCharacterSize(characterSize);

			sf::FloatRect rc = buttonText.getLocalBounds();
			buttonText.setOrigin(rc.width / 2, rc.height / 2);
			buttonText.setPosition(position);
			buttonText.setRotation(angle);
		};

	initScrollBarButton(upButton, sf::Vector2f(SCROLLBAR_X, SCROLLBAR_Y + 2));
	initScrollBarButton(downButton, sf::Vector2f(PANEL_WIDTH + PANEL_MARGIN_X + 2, PANEL_HEIGHT + TOP_BUTTONS_HEIGHT));

	initScrollBarButtonText(buttonText, 13, "V", sf::Vector2f(SCROLLBAR_X + SCROLLBAR_WIDTH / 2, SCROLLBAR_Y + SCROLLBAR_BUTTON_HEIGHT / 1.25f), 180.f);
	initScrollBarButtonText(buttonText2, 13, "V", sf::Vector2f(downButton.getPosition().x + SCROLLBAR_WIDTH / 2, downButton.getPosition().y + SCROLLBAR_BUTTON_HEIGHT / 3), 0);

	window.draw(upButton), window.draw(downButton);
	window.draw(buttonText), window.draw(buttonText2);
}

void App::drawButtons() {
	for (unsigned int index = 0; index < buttons.size(); ++index)
		buttons[index].draw();
	drawScrollbarButtons();
}

void App::getCursor(sf::Cursor& cursor) {
	cursor.loadFromSystem(sf::Cursor::Hand);
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	if (mousePosition.y >= 0 && mousePosition.y <= TOP_BUTTONS_HEIGHT + 15)
		cursor.loadFromSystem(sf::Cursor::Arrow);
	if (mousePosition.y >= TOP_BUTTONS_HEIGHT + PANEL_HEIGHT - BOTTOM_BUTTONS_HEIGHT + 15 && mousePosition.y <= window.getSize().y)
		cursor.loadFromSystem(sf::Cursor::Arrow);
	if (leftPanel.checkMouseOnFolder(leftPanel.getSelectedFolderIndex(), (float)mousePosition.x, (float)mousePosition.y) || rightPanel.checkMouseOnFolder(rightPanel.getSelectedFolderIndex(), (float)mousePosition.x, (float)mousePosition.y))
		cursor.loadFromSystem(sf::Cursor::Arrow);
}