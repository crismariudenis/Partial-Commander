#include "Panel.h"


void Panel::init(sf::Vector2f pos, int width, int height, std::filesystem::path currentPath, std::vector<sf::Font>& fonts) {
	this->pos = pos;
	this->width = width;
	this->height = height;
	this->currentPath = currentPath;
	this->fonts = fonts;
	update(currentPath);
	initColumnTitles();
	initBorders();
	initCurrentPath();
}

void Panel::initBorders() {

	panelRect.setOutlineThickness(PANEL_LINE_WIDTH);
	panelRect.setPosition(pos);
	panelRect.setSize(sf::Vector2f(1.f * width, 1.f * height));
	panelRect.setOutlineColor(sf::Color::White);
	panelRect.setFillColor(sf::Color::Transparent);

	line1.setOutlineThickness(PANEL_LINE_WIDTH / 2);
	line1.setPosition(pos + sf::Vector2f(FOLDER_SPACE, 0));
	line1.setSize(sf::Vector2f(0, 1.f * height - PANEL_BOTTOM_HEIGHT));
	line1.setOutlineColor(sf::Color::White);

	line2.setOutlineThickness(PANEL_LINE_WIDTH / 2);
	line2.setPosition(pos + sf::Vector2f(FOLDER_SPACE + SIZE_SPACE, 0));
	line2.setSize(sf::Vector2f(0, 1.f * height - PANEL_BOTTOM_HEIGHT));
	line2.setOutlineColor(sf::Color::White);

	line3.setOutlineThickness(PANEL_LINE_WIDTH / 2);
	line3.setPosition(pos + sf::Vector2f(0, PANEL_HEIGHT - PANEL_BOTTOM_HEIGHT));
	line3.setSize(sf::Vector2f(1.f * width, 0));
	line3.setOutlineColor(sf::Color::White);
}

void Panel::initColumnTitles() {

	folderName.setString("Name");
	sf::FloatRect rc = folderName.getLocalBounds();
	folderName.setCharacterSize(CHARACTER_SIZE);
	folderName.setFillColor(titleColor);
	folderName.setOrigin(rc.width / 2, 0);
	folderName.setFont(fonts[CustomFonts::Font::UBUNTU]);
	folderName.setPosition(pos + sf::Vector2f(FOLDER_SPACE / 2, 10));

	sizeName = folderName;
	sizeName.setString("Size");
	rc = sizeName.getLocalBounds();
	sizeName.setOrigin(rc.width / 2, 0);
	sizeName.setPosition(pos + sf::Vector2f(FOLDER_SPACE + SIZE_SPACE / 2, 10));

	dateName = sizeName;
	dateName.setString("Modify Time");
	rc = dateName.getLocalBounds();
	dateName.setOrigin(rc.width / 2, 0);
	dateName.setPosition(pos + sf::Vector2f(FOLDER_SPACE + SIZE_SPACE + TIME_SPACE / 2, 10));
}

void Panel::initCurrentPath() {
	std::string s = currentPath.string();
	if (s.size() > 40)
		s = "..." + s.substr(s.size() - 40, s.size());

	currentPathText.setString(s);
	if (currentPathText.getFont() == NULL) {
		currentPathText.setFillColor(textColor);
		currentPathText.setPosition(sf::Vector2f(pos.x + PANEL_MARGIN_X, PANEL_HEIGHT - PANEL_BOTTOM_HEIGHT + PANEL_MARGIN_TOP + 12.5));
		currentPathText.setCharacterSize(CHARACTER_SIZE + 5);
		currentPathText.setFont(fonts[CustomFonts::Font::UBUNTU]);
	}
}

void Panel::draw() {
	if (isSelected) {
		/* Scrollbar */
		window.draw(scrollbar);
		drawSelectedFolderBackground();
		drawShortcutText();
	}

	/* Folders */
	drawFolders();

	/* Borders */
	window.draw(panelRect);
	window.draw(line1);
	window.draw(line2);
	window.draw(line3);

	/* Column Titles */
	window.draw(folderName);
	window.draw(sizeName);
	window.draw(dateName);

	if (!isSearchActive) {
		drawCurrentPath();
		drawFreeSpace();
	}
	else drawSearchText();
}

void Panel::drawFolders() {
	for (int index = firstToDisplay; index <= lastToDisplay; ++index) {
		folders[index].position.y = 1.f * (index - firstToDisplay + 1) * (1.f * height / LINE_SPACING) + pos.y + TOP_TEXT_BORDER;
		folders[index].updateText();
		window.draw(folders[index].folderText);
		window.draw(folders[index].sizeText);
		window.draw(folders[index].dateText);
	}
}

void Panel::drawSelectedFolderBackground() {
	for (int index = firstToDisplay; index <= lastToDisplay; ++index) {
		if (index == selectedFolderIndex || shortcutSelectedFolder[index]) {
			sf::Vector2f topLeft = folders[index].position;
			sf::Vector2f botRight(width - 13.f, LINE_SPACING);
			sf::RectangleShape rect;
			rect.setSize(botRight);
			rect.setFillColor(secondaryColor);
			rect.setPosition(topLeft);
			window.draw(rect);
		}
	}
}

void Panel::drawShortcutText() {
	if (!pressedKeys || isSearchActive)
		return;
	std::string shortcutString;
	std::vector < std::pair<int, std::string >> timeStamps;
	for (auto it : pressed) {
		if (it.second) {
			std::string command;
			if (it.first == sf::Keyboard::Scan::LControl || it.first == sf::Keyboard::Scan::LShift) {
				if (it.first == sf::Keyboard::Scan::LControl)
					command += "CTRL ";
				else command += "SHIFT ";
			}
			if (it.first >= sf::Keyboard::Scan::Num1 && it.first <= sf::Keyboard::Scan::Num0 || it.first >= sf::Keyboard::Scan::F1 && it.first <= sf::Keyboard::Scan::F9) {
				if (it.first >= sf::Keyboard::Scan::F1 && it.first <= sf::Keyboard::Scan::F9)
					command += "F" + std::to_string(it.first - sf::Keyboard::Scan::F1 + 1) + ' ';
				else command += std::to_string((it.first - sf::Keyboard::Scan::Num1) % 10) + ' ';
			}
			if (it.first >= sf::Keyboard::Scan::A && it.first <= sf::Keyboard::Scan::Z)
				command += (char)(it.first + 'A'), command += ' ';
			if (it.first == sf::Keyboard::Scan::Up)
				command += "UP ";
			if (it.first == sf::Keyboard::Scan::Down)
				command += "DOWN ";
			if (it.first == sf::Keyboard::Scan::Enter)
				command += "ENTER ";
			if (command.size() > 0) timeStamps.push_back({ it.second, command });
		}
	}

	std::sort(timeStamps.begin(), timeStamps.end());
	for (std::pair<int, std::string> it : timeStamps)
		shortcutString += it.second;

	shortcutText.setString(shortcutString);
	sf::FloatRect bounds = shortcutText.getLocalBounds();
	shortcutText.setPosition(sf::Vector2f((WINDOW_WIDTH - bounds.width) / 2, WINDOW_HEIGHT - BOTTOM_BUTTONS_HEIGHT / 1.5));
	shortcutText.setCharacterSize(17);
	shortcutText.setFont(fonts[CustomFonts::Font::UBUNTU]);
	shortcutText.setFillColor(textColor);
	window.draw(shortcutText);
}

void Panel::drawFreeSpace()
{
	const std::filesystem::space_info si = std::filesystem::space(currentPath);
	unsigned long long space = si.capacity;
	unsigned long long spaceUsed = space - si.free;
	int order = 0;
	while (space > 999) space >>= 10, order++;
	while (spaceUsed > 999) spaceUsed >>= 10;
	sf::Text spaceText;

	std::string spaceString = std::to_string(spaceUsed) + "/" + std::to_string(space);
	if (order == 1)
		spaceString += " KB ";
	else if (order == 2)
		spaceString += " MB ";
	else if (order == 3)
		spaceString += " GB ";
	else
		spaceString += " B ";

	uint64_t raport = (spaceUsed * 100) / space;
	spaceString += '(' + std::to_string(raport) + "%)";

	spaceText.setString(spaceString);
	spaceText.setFillColor(textColor);
	spaceText.setPosition(sf::Vector2f(pos.x + PANEL_MARGIN_X + PANEL_WIDTH - 220, PANEL_HEIGHT - PANEL_BOTTOM_HEIGHT + PANEL_MARGIN_TOP + 12.5));
	spaceText.setCharacterSize(CHARACTER_SIZE + 5);
	spaceText.setFont(fonts[CustomFonts::Font::UBUNTU]);
	window.draw(spaceText);
}

void Panel::drawSearchText() {
	std::string searchString = searchText.getString();
	searchString += '|';
	searchText.setString(searchString);
	searchText.setFillColor(textColor);
	searchText.setPosition(sf::Vector2f(pos.x + PANEL_MARGIN_X, PANEL_HEIGHT - PANEL_BOTTOM_HEIGHT + PANEL_MARGIN_TOP + 12.5));
	searchText.setCharacterSize(CHARACTER_SIZE + 5);
	searchText.setFont(fonts[CustomFonts::Font::UBUNTU]);
	window.draw(searchText);
	searchString.pop_back();
	searchText.setString(searchString);
}

void Panel::drawCurrentPath() {
	std::string s = currentPathText.getString();
	if (s.size() > 40)
		currentPathText.setString("..." + s.substr(s.size() - 40, 40));
	currentPathText.setFillColor(textColor);
	window.draw(currentPathText);
	currentPathText.setString(s);
}

void Panel::update(std::filesystem::path path) {

	if (!std::filesystem::exists(path))
		return;

	this->currentPath = path;
	sf::Vector2f textPosition = pos;
	textPosition.x += 10;
	textPosition.y += 1.f * height / LINE_SPACING + TOP_TEXT_BORDER;
	firstToDisplay = 0, lastToDisplay = 1, selectedFolderIndex = 0;

	std::vector <Folder> undefinedFolders;
	folders.clear();
	folders.push_back(Folder("/..", textPosition, fonts));

	quadrants = { {pos.x, pos.y, pos.x + FOLDER_SPACE, pos.y + 38.f},
				  {pos.x + FOLDER_SPACE + 1, pos.y, pos.x + FOLDER_SPACE + SIZE_SPACE, pos.y + 38.f},
				  {pos.x + FOLDER_SPACE + SIZE_SPACE + 1, pos.y, pos.x + width, pos.y + 38.f} };

	for (auto const& entry : std::filesystem::directory_iterator(path)) {
		if (!std::filesystem::exists(entry))
			continue;
		if (!std::filesystem::is_directory(entry) && !std::filesystem::is_regular_file(entry))
			continue;
		try {
			entry.path().string();
		}
		catch (const std::exception& e) {
			std::cerr << "Exception caught: " << e.what() << std::endl;
			continue;
		}
		std::string prefix = entry.path().stem().string().substr(0, 10);
		if (prefix == "New Folder") 
			undefinedFolders.push_back(Folder(entry.path(), textPosition, fonts));
		else {
			textPosition.y += 1.f * height / LINE_SPACING;
			folders.push_back(Folder(entry.path(), textPosition, fonts));
			if (textPosition.y <= height - PANEL_OFFSET - 20) {
				lastToDisplay++;
			}
		}
	}
	for (int index = 0; index < undefinedFolders.size(); ++index) {
		textPosition.y += 1.f * height / LINE_SPACING;
		folders.push_back(undefinedFolders[index]);
		if (textPosition.y <= height - PANEL_OFFSET - 20) 
			lastToDisplay++;
	}
	lastToDisplay--;
	float scrollPerUnit = 1.f * (height - 2.f * SCROLLBAR_BUTTON_HEIGHT) / folders.size();
	scrollbar.init(SCROLLBAR_WIDTH, scrollPerUnit * (lastToDisplay - firstToDisplay + 1) + 4.5f, scrollPerUnit);
	foldersCopy = folders;
	folders[selectedFolderIndex].toggleIsSelected();
}

void Panel::updateSelectedFolder(sf::Keyboard::Scancode code) {
	if (isSelected && isDirectoryLabelActive)
	{
		switch (code)
		{
		case sf::Keyboard::Scancode::S:
		case sf::Keyboard::Scancode::Down:
		{
			if (selectedFolderIndex + 1 < (int)folders.size()) 
				updateFolderSelectedFolder(selectedFolderIndex + 1);
			if (selectedFolderIndex == lastToDisplay) {
				int step = std::min((size_t)10, folders.size() - lastToDisplay - 1);
				firstToDisplay += step;
				lastToDisplay += step;
				scrollbar.move(0, step);
			}
			break;
		}
		case sf::Keyboard::Scancode::W:
		case sf::Keyboard::Scancode::Up:
		{
			if (selectedFolderIndex != 0)
				updateFolderSelectedFolder(selectedFolderIndex - 1);
			if (selectedFolderIndex == firstToDisplay) {
				int step = std::min(10, firstToDisplay);
				firstToDisplay -= step;
				lastToDisplay -= step;
				scrollbar.move(0, -step);
			}
			break;
		}
		}
	}
}

void Panel::updateShortcutSelectedFolder(int type, int move)
{
	if (!isSelected || !isDirectoryLabelActive)
		return;
	shortcutSelectedFolder[selectedFolderIndex] = true;
	if (type == 1) { /// Ctrl A
		for (unsigned int i = 0; i < folders.size(); ++i)
			shortcutSelectedFolder[i] = true;
	}
	else if (type == 2) { /// Ctrl Shift Up/Down
		if (shiftSelectedFolder == -1)
			shiftSelectedFolder = selectedFolderIndex;
		if (shiftSelectedFolder - selectedFolderIndex > 0) {
			if (move > 0 && shiftSelectedFolder + 1 < (int)folders.size()) shortcutSelectedFolder[++shiftSelectedFolder] = true;
			else if (move < 0) shortcutSelectedFolder[shiftSelectedFolder--] = false;
		}
		else if (shiftSelectedFolder - selectedFolderIndex < 0) {
			if (move > 0) shortcutSelectedFolder[shiftSelectedFolder++] = false;
			else if (shiftSelectedFolder > 0) shortcutSelectedFolder[--shiftSelectedFolder] = true;
		}
		else if (shiftSelectedFolder == selectedFolderIndex) {
			if (move > 0) shortcutSelectedFolder[++shiftSelectedFolder] = true;
			else shortcutSelectedFolder[--shiftSelectedFolder] = true;
		}
	}
	else if (type == 3) {

		int size = 0;
		for (auto pos : shortcutSelectedFolder)
			if (pos.second) size++;

		shortcutSelectedFolder.clear();
		if (shiftSelectedFolder != -1)
			updateFolderSelectedFolder(std::min(shiftSelectedFolder, lastToDisplay));
		else if (size > 1)
			updateFolderSelectedFolder(lastToDisplay);

		shiftSelectedFolder = -1;
		folders[selectedFolderIndex].folderText.setOutlineThickness(0);
	}
	else if (type == 4) { // Control LMouse
		move += firstToDisplay;
		if (move < (int)folders.size()) {
			if (shortcutSelectedFolder[move] == true)
				shortcutSelectedFolder[move] = false;
			else
				shortcutSelectedFolder[move] = true;
		}
	}
	if (shiftSelectedFolder == firstToDisplay) {
		int step = std::min(10, firstToDisplay);
		firstToDisplay -= step;
		lastToDisplay -= step;
	}

	if (shiftSelectedFolder == lastToDisplay) {
		int step = std::min((size_t)10, folders.size() - lastToDisplay - 1);
		firstToDisplay += step;
		lastToDisplay += step;
	}
}

void Panel::updateFolderSelectedFolder(int newSelectedFolder) {
	folders[selectedFolderIndex].toggleIsSelected();
	folders[selectedFolderIndex].updateText();
	selectedFolderIndex = newSelectedFolder;
	folders[selectedFolderIndex].toggleIsSelected();
	folders[selectedFolderIndex].updateText();
}

void Panel::updateByScrollbar(sf::Vector2f mouse)
{
	float x = scrollbar.getPosition().x, y = scrollbar.getPosition().y;
	int steps = (int)std::min(folders.size() - 1.f * lastToDisplay - 1, (mouse.y - y) / scrollbar.getUnitPerFolder());
	if (mouse.y - y < 0)
		steps = -(int)std::min(1.f * firstToDisplay, (y - mouse.y) / scrollbar.getUnitPerFolder());
	updateByScrollbar(steps, 2);
}

void Panel::updateByScrollbar(int steps, int type)
{
	if (!isSelected && lastToDisplay)
		return;
	if (!firstToDisplay && steps < 0)
		return;
	if (lastToDisplay + 1 == folders.size() && steps > 0)
		return;

	if (type == 1) {
		if (shiftSelectedFolder + steps == firstToDisplay) {
			int step = std::min(10, firstToDisplay);
			firstToDisplay -= step;
			lastToDisplay -= step;
			scrollbar.move(0, -step);
		}
		else if (shiftSelectedFolder + steps == lastToDisplay) {
			int step = std::min((size_t)10, folders.size() - lastToDisplay - 1);
			firstToDisplay += step;
			lastToDisplay += step;
			scrollbar.move(0, step);
		}
	}
	else if (type == 2) {
		lastToDisplay += steps;
		firstToDisplay += steps;
		scrollbar.move(0, steps);
	}
}

void Panel::updateFoldersByFilter() {
	folders.clear();
	int lastToDisplay2 = -1;
	for (unsigned int index = 0; index < foldersCopy.size(); ++index) {

		if (foldersCopy[index].folderText.getString() == "/..") {
			folders.push_back(foldersCopy[index]);
			lastToDisplay2++;
			continue;
		}
		if (isValidByFilter(foldersCopy[index].folderText.getString())) {
			folders.push_back(foldersCopy[index]);
			lastToDisplay2++;
		}
	}
	shortcutSelectedFolder.clear();
	int dif = selectedFolderIndex - firstToDisplay;
	lastToDisplay = std::min(17, lastToDisplay2);
	firstToDisplay = std::max(lastToDisplay - 17, 0);
	selectedFolderIndex = std::min(firstToDisplay + dif, lastToDisplay);
	folders[selectedFolderIndex].toggleIsSelected();

	float scrollPerUnit = 1.f * (height - 2.0 * SCROLLBAR_BUTTON_HEIGHT) / folders.size();
	scrollbar.init(SCROLLBAR_WIDTH, scrollPerUnit * (lastToDisplay - firstToDisplay + 1) + 4.5f, scrollPerUnit);

}

void Panel::updateColors() {
	initColumnTitles();
	initBorders();
	initCurrentPath();
}

void Panel::changePath(int type) {
	if (type == 2) {
		std::filesystem::path path = currentPathText.getString().toAnsiString();
		update(path);
		currentPathText.setString(currentPath.string());
		return;
	}
	auto& p = folders[selectedFolderIndex].path;
	if (!std::filesystem::is_directory(p))
	{
		std::string realPath = p.string();

		// Add "" around the spaces in a path
		// Ex:  D:\test (1).jpg  ->  D:\test" "(1).jpg
		for (size_t i = 0; i < realPath.size(); i++)
			if (realPath[i] == ' ') {
				realPath = realPath.substr(0, i) + "\" \"" + realPath.substr(i + 1, realPath.size());
				i += 2;
			}

		std::string cmd = "start " + realPath;
		try {
			system(cmd.c_str());
		}
		catch (const std::exception& e) {
			std::cerr << "Exception caught: " << e.what() << std::endl;
		}
		return;
	}
	std::filesystem::path folderPath;
	if (selectedFolderIndex)
			folderPath = p;
		else folderPath = currentPath.parent_path();
	update(folderPath);
	currentPathText.setString(currentPath.string());
}

void Panel::checkTextLabels(sf::Vector2f mouse) {

	std::vector<sf::Vector2f> initialPositions;
	auto initPositions = [](std::vector<Folder>& folders) {
		std::vector<sf::Vector2f> initialPositions;
		for (unsigned int index = 0; index < folders.size(); ++index)
			initialPositions.push_back(folders[index].position);
		return initialPositions;
		};

	for (unsigned int index = 0; index < quadrants.size(); ++index) {
		if (checkBoxLabel(std::get<0>(quadrants[index]), std::get<1>(quadrants[index]), std::get<2>(quadrants[index]), std::get<3>(quadrants[index]), mouse.x, mouse.y)) {
			initialPositions = initPositions(folders);
			folders[selectedFolderIndex].toggleIsSelected(), folders[selectedFolderIndex].updateText();
			if (abs(sortType) == index + 1) {
				reverse(folders.begin() + 1, folders.end());
				sortType = -sortType;
			}
			else {
				if (index == 0) std::sort(folders.begin() + 1, folders.end(), nameCompare);
				else if (index == 1) std::sort(folders.begin() + 1, folders.end(), sizeCompare);
				else std::sort(folders.begin() + 1, folders.end(), timeCompare);
				sortType = index + 1;
			}
			folders[selectedFolderIndex].toggleIsSelected(), folders[selectedFolderIndex].updateText();
		}
	}
}

void Panel::checkFolderLabels(sf::Vector2f mouse) {
	bool ok = false;
	for (int index = firstToDisplay; isSelected && index <= lastToDisplay && !ok; ++index) {
		ok = checkMouseOnFolder(index, mouse.x, mouse.y);
		if (ok) {
			updateShortcutSelectedFolder(3, -1);
			updateFolderSelectedFolder(index);
		}
	}
}

bool Panel::checkMouseOnFolder(int index, float mouseX, float mouseY) {
	Folder folder = folders[index];
	double extraHeight = 1.f * height / LINE_SPACING;
	return checkBoxLabel(folder.folderText.getPosition().x, folder.folderText.getPosition().y,
		folder.folderText.getPosition().x + PANEL_WIDTH - PANEL_MARGIN_X, folder.folderText.getPosition().y + extraHeight, mouseX, mouseY);
}

bool Panel::checkScrollbarLabel(sf::Vector2f mouse) {
	return checkBoxLabel(scrollbar.getPosition().x, scrollbar.getPosition().y,
		scrollbar.getPosition().x + scrollbar.getSize().x, scrollbar.getPosition().y + scrollbar.getSize().y, mouse.x, mouse.y);
}

void Panel::resetTextPositions(std::vector<sf::Vector2f> initialPositions) {
	for (unsigned int index = 0; index < folders.size(); ++index) {
		folders[index].position = initialPositions[index];
		if (index == selectedFolderIndex)
			folders[index].toggleIsSelected();
		folders[index].updateText();
	}
}

void Panel::resetFoldersPositions() {
	sf::Vector2f textPosition = sf::Vector2f(pos.x, pos.y + TOP_TEXT_BORDER);
	textPosition.x += 10;
	textPosition.y += 1.f * height / LINE_SPACING;
	for (unsigned int index = 0; index < folders.size(); ++index) {
		folders[index].position = textPosition;
		textPosition.y += 1.f * height / LINE_SPACING;
	}
}

void Panel::registerCharacter(int scancode, bool isUpperCase, int type) {
	if (!isSelected)
		return;
	std::vector<std::string> words = { searchText.getString(), folders[selectedFolderIndex].folderText.getString(), currentPathText.getString().toAnsiString()};
		if (scancode >= 0 && scancode <= 25) { /// Alphabet
		scancode += 'a';
		if (isUpperCase) scancode -= 'a' - 'A';
		words[type - 1] += (char)(scancode);
	}
	else if (scancode == 38) {  /// Delete 
		if (words[type - 1].size() > 0)
			words[type - 1].pop_back();
	}
	else if (scancode >= sf::Keyboard::Scan::Num1 && scancode <= sf::Keyboard::Scan::Num0) { /// Digits
		char digit = (scancode - sf::Keyboard::Scan::Num1 + 1) % 10 + '0';
;		if ((digit == '0' || digit == '9') && isUpperCase) 
			digit = (digit == '0' ? ')' : '(');
		words[type - 1] += digit;
	}
	else { /// Special characteres : '_', '-'
		for (std::pair<int, char> ch : specialCharacters) {
			if (ch.first == scancode) {
				words[type - 1] += ch.second;
			}
		}
	}
	if (type == 1 && isSearchActive && !isDirectoryLabelActive) { /// CTRL F - Search Function
		searchText.setString(words[0]);
		updateFoldersByFilter();
	}
	else if (type == 2 && isDirectoryLabelActive) { /// CTRL R - Rename Function
		folders[selectedFolderIndex].folderText.setString(words[1]);
		folders[selectedFolderIndex].folderText.setOutlineColor(outlineColor);
		folders[selectedFolderIndex].folderText.setOutlineThickness(3);
	}
	else if (type == 3) { /// CTRL SHIFT M - Manual path change
		currentPathText.setString(words[2]);
	}
}

bool Panel::isValidByFilter(std::string folderText) {
	std::string search = searchText.getString().toAnsiString();
	if (search.size() > folderText.size())
		return false;
	if (search.size() == 0)
		return true;

	for (unsigned int index = 0; index < folderText.size(); ++index) {
		if (folderText[index] >= 'a' && folderText[index] <= 'z')
			folderText[index] = folderText[index] - 'a' + 'A';
		if (index < search.size() && search[index] >= 'a' && search[index] <= 'z')
			search[index] = search[index] - 'a' + 'A';
		if (index + 1 >= search.size()) {
			std::string elector = folderText.substr(index - search.size() + 1, search.size());
			if (elector == search)
				return true;
		}
	}
	return false;
}

void Panel::activateLabel(int mouseX, int mouseY) {
	for (unsigned int index = 0; index < quadrants.size(); ++index) {
		if (checkBoxLabel(std::get<0>(quadrants[index]), std::get<1>(quadrants[index]), std::get<2>(quadrants[index]), std::get<3>(quadrants[index]), 1.f * mouseX, 1.f * mouseY)) {
			sf::RectangleShape background;
			initBackground(background, sortLabelColor, sf::Vector2f(std::get<0>(quadrants[index]), pos.y), sf::Vector2f(std::get<2>(quadrants[index]) - std::get<0>(quadrants[index]), 36));
			window.draw(background);
		}
	}
}

void Panel::activateSearch() {
	if (isSearchActive) {
		registerCharacter(38, 1, 1);
		isSearchActive = false;
		isDirectoryLabelActive = true;
		return;
	}
	isSearchActive = true;
	isDirectoryLabelActive = false;
	updateFoldersByFilter();
}

void Panel::changeDirectory(std::filesystem::path p) {
	if (isSelected)
		update(p);
	initCurrentPath();
}

void Panel::toggleIsSelected() {
	isSelected = !isSelected;
	folders[selectedFolderIndex].toggleIsSelected();
	folders[selectedFolderIndex].updateText();
}

void Panel::setSelectedFolder(std::string text) {
	folders[selectedFolderIndex].folderText.setString(text);
}

Folder Panel::getSelectedFolder() {
	return folders[selectedFolderIndex];
}

std::vector<Folder>& Panel::getFolders() {
	return folders;
}

std::filesystem::path Panel::getCurrentPath() {
	return currentPath;
}

int Panel::getSelectedFolderIndex() {
	return selectedFolderIndex;
}