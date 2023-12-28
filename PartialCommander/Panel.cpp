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

void Panel::drawFolders() {
	for (int index = firstToDisplay; index <= lastToDisplay; ++index) {
		folders[index].position.y = 1.f * (index - firstToDisplay + 1) * (1.f * height / LINE_SPACING) + pos.y + TOP_TEXT_BORDER;
		folders[index].updateText();
		window.draw(folders[index].folderText);
		window.draw(folders[index].sizeText);
		window.draw(folders[index].dateText);
	}
}

void Panel::drawBorders() {
	window.draw(panelRect);
	window.draw(line1);
	window.draw(line2);
	window.draw(line3);
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
	line2.setSize(sf::Vector2f(0, 1.f*height - PANEL_BOTTOM_HEIGHT));
	line2.setOutlineColor(sf::Color::White
	);

	line3.setOutlineThickness(PANEL_LINE_WIDTH / 2);
	line3.setPosition(pos + sf::Vector2f(0, PANEL_HEIGHT - PANEL_BOTTOM_HEIGHT));
	line3.setSize(sf::Vector2f(1.f*width, 0));
	line3.setOutlineColor(sf::Color::White);
}

void Panel::drawColumnTitles() {
	window.draw(folderName);
	window.draw(sizeName);
	window.draw(dateName);
}

void Panel::initColumnTitles() {

	folderName.setString("Name");
	sf::FloatRect rc = folderName.getLocalBounds();
	folderName.setCharacterSize(CHARACTER_SIZE);
	folderName.setFillColor(titleColor);
	folderName.setOrigin(rc.width / 2, 0);
	folderName.setFont(fonts[CustomFonts::Font::ROBOTO]);
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

void Panel::draw() {
	if (isSelected) {
		drawScrollbar();
		drawSelectedFolderBackground();
	}
	drawFolders();
	drawBorders();
	drawColumnTitles();
	if (!isSearchActive) {
		drawCurrentPath();
		drawFreeSpace();
	}
	else drawSearchText();
}

void Panel::drawScrollbar() {
	window.draw(scrollbar);
}
void Panel::update(std::filesystem::path path) {

	if (!std::filesystem::exists(path))
		return;

	this->currentPath = path;
	sf::Vector2f textPosition = pos;
	textPosition.x += 10;
	textPosition.y += 1.f * height / LINE_SPACING + TOP_TEXT_BORDER;
	firstToDisplay = 0, lastToDisplay = 1, selectedFolderIndex = 0;

	folders.clear();
	folders.push_back(Folder("/..", textPosition, fonts, " "));
	
	quadrants = { {pos.x, pos.y, pos.x + FOLDER_SPACE, pos.y + 20.f},
				  {pos.x + FOLDER_SPACE + 1, pos.y, pos.x + FOLDER_SPACE + SIZE_SPACE, pos.y + 20.f},
				  {pos.x + FOLDER_SPACE + SIZE_SPACE + 1, pos.y, pos.x + width, pos.y + 20.f} };

	for (auto const& entry : std::filesystem::directory_iterator(path)) {
		if(!std::filesystem::exists(entry))
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
		textPosition.y += 1.f * height / LINE_SPACING;
		folders.push_back(Folder(entry.path(), textPosition, fonts, getDate(entry.path())));
		if (textPosition.y <= height - PANEL_OFFSET - 20) {
			lastToDisplay++;
		}

	}
	lastToDisplay--;
	float scrollPerUnit = 1.f * (height - 2.f * SCROLLBAR_BUTTON_HEIGHT) / folders.size();
	scrollbar.init(SCROLLBAR_WIDTH, scrollPerUnit * (lastToDisplay - firstToDisplay + 1) + 4.5f, scrollPerUnit);
	foldersCopy = folders;
}
std::string Panel::getDate(std::filesystem::path path) {
	// Convert from fs::file_time_type to std::time_t
	auto to_time_t = [](auto tp) {
		namespace cs = std::chrono;
		namespace fs = std::filesystem;
		auto sctp = cs::time_point_cast<cs::system_clock::duration>(tp -
			fs::file_time_type::clock::now() + cs::system_clock::now());
		return cs::system_clock::to_time_t(sctp);
		};


	auto fileTime = std::filesystem::last_write_time(path);
	std::time_t tt = to_time_t(fileTime);

	// Convert the std::time_t value to a local time structure
	std::tm* localTime = std::localtime(&tt);
	std::stringstream buff;

	buff << std::put_time(localTime, "%d  %b  %H:%M");
	std::string formattedDate = buff.str();
	return formattedDate;
}

void Panel::updateSelectedFolder(sf::Keyboard::Scancode code) {

	if (isSelected && isDirectoryLabelActive) {
		switch (code)
		{
		case sf::Keyboard::Scancode::S:
		case sf::Keyboard::Scancode::Down:
		{
			if (selectedFolderIndex + 1 - folders.size() < 0) {
				updateFolderSelectedFolder(selectedFolderIndex + 1);
			}
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

		case sf::Keyboard::Scancode::F8: 
		case sf::Keyboard::Scancode::Backspace:
		{
			for (unsigned int index = 0; index < folders.size(); ++index) {
				if (index == selectedFolderIndex || shortcutSelectedFolder[index])
					sys->del(folders[index].path);
			}
			update(currentPath);
			updateShortcutSelectedFolder(3, -1);
			break;
		}
		case sf::Keyboard::Scancode::F5: {
			for (unsigned int index = 0; index < folders.size(); ++index) {
				if (index == selectedFolderIndex || shortcutSelectedFolder[index])
					sys->copy(folders[index].path, folders[index].path.parent_path());
			}
			update(currentPath);
			updateShortcutSelectedFolder(3, -1);
			break;
		}
		default:
			break;
		}
	}
}

void Panel::toggleIsSelected() {
	isSelected = !isSelected;
	folders[selectedFolderIndex].toggleIsSelected();
	folders[selectedFolderIndex].updateText();
}

void Panel::changePath() {

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
			std::cout << cmd.c_str() << '\n';
		system(cmd.c_str());
		return;
	}
	if (isSelected) {
		std::filesystem::path folderPath;
		if (selectedFolderIndex)
			folderPath = p;
		else
			folderPath = currentPath.parent_path();
		std::cout << folderPath.string() << '\n';
		update(folderPath);
	}
}

void Panel::changeDirectory(std::filesystem::path p) {
	std::cout << p.string() << '\n';
	if (isSelected)
		update(p);
}

void Panel::initCurrentPath() {
	currentPathText.setString(currentPath.string());
	currentPathText.setFillColor(textColor);
	currentPathText.setPosition(sf::Vector2f(pos.x + PANEL_MARGIN_X, PANEL_HEIGHT + BOTTOM_BUTTONS_HEIGHT / 3));
	currentPathText.setCharacterSize(CHARACTER_SIZE + 5);
	currentPathText.setFont(fonts[CustomFonts::Font::ROBOTO]);
}

void Panel::drawCurrentPath() {
	window.draw(currentPathText);
}

bool Panel::checkBoxLabel(float topLeftX, float topLeftY, float botRightX, float botRightY, float mouseX, float mouseY)
{
	return mouseX >= topLeftX && mouseX <= botRightX && mouseY >= topLeftY && mouseY <= botRightY;
}

bool nameCompare(const Folder& a, const Folder& b) {
	std::string name1 = a.folderText.getString().toAnsiString();
	std::string name2 = b.folderText.getString().toAnsiString();
	return name1 < name2;
}

bool sizeCompare(const Folder& a, const Folder& b) {
	return a.getSize() < b.getSize();
}

bool timeCompare(const Folder& a, const Folder& b) {
	std::filesystem::file_time_type ftime1 = std::filesystem::last_write_time(a.path);
	std::filesystem::file_time_type ftime2 = std::filesystem::last_write_time(b.path);
	return ftime1 < ftime2;
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
		}
	}
}

void Panel::checkFolderLabels(sf::Vector2f mouse) {
	for (int index = firstToDisplay; isSelected && index <= lastToDisplay; ++index) {
		bool ok = checkMouseOnFolder(index, mouse.x, mouse.y);
		if (ok) {
			updateFolderSelectedFolder(index);
			return;
		}
	}
}

void Panel::resetTextPositions(std::vector<sf::Vector2f> initialPositions) {
	for (unsigned int index = 0; index < folders.size(); ++index) {
		folders[index].position = initialPositions[index];
		if (index == selectedFolderIndex)
			folders[index].toggleIsSelected();
		folders[index].updateText();
	}
}

void Panel::activateLabel(int mouseX, int mouseY) {
	for (unsigned int index = 0; index < quadrants.size(); ++index) {
		if (checkBoxLabel(std::get<0>(quadrants[index]), std::get<1>(quadrants[index]), std::get<2>(quadrants[index]), std::get<3>(quadrants[index]), 1.f*mouseX, 1.f*mouseY)) {
			sf::RectangleShape background;
			initBackground(background, sortLabelColor, sf::Vector2f(std::get<0>(quadrants[index]), pos.y), sf::Vector2f(std::get<2>(quadrants[index]) - std::get<0>(quadrants[index]), 36));
			window.draw(background);
		}
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

int Panel::getSelectedFolderIndex() {
	return selectedFolderIndex;
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
			if (move > 0 && shiftSelectedFolder + 1 - folders.size() < 0) shortcutSelectedFolder[++shiftSelectedFolder] = true;
			else if(move < 0) shortcutSelectedFolder[shiftSelectedFolder--] = false;
		}
		else if (shiftSelectedFolder - selectedFolderIndex < 0 ) {
			if (move > 0) shortcutSelectedFolder[shiftSelectedFolder++] = false;
			else if(shiftSelectedFolder > 0) shortcutSelectedFolder[--shiftSelectedFolder] = true;
		}
		else if(shiftSelectedFolder == selectedFolderIndex) {
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
			updateFolderSelectedFolder(shiftSelectedFolder);
		else if (size > 1)
			updateFolderSelectedFolder(lastToDisplay);

		shiftSelectedFolder = -1;
	}
	else if (type == 4) { // Control LMouse
		move += firstToDisplay;
		if (move - folders.size() < 0) {
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

void Panel::updateClipboard() {
	Clipboard* clipboard = Clipboard::getInstance();
	shortcutSelectedFolder[selectedFolderIndex] = true;
	if (isSelected)
		clipboard->copy(shortcutSelectedFolder, folders);
}

void Panel::pasteFromClipboard(std::vector<Folder> folders) {
	/*std::string extension = path.extension().string();
	for (int i = 1; std::filesystem::exists(destPath); i++) {
		destPath = path.parent_path() / (path.stem().string() + std::to_string(i) + extension);
	}*/
	if (isSelected) {
		int copyIndex = selectedFolderIndex;
		for (size_t index = 0; index < folders.size(); ++index) 
			sys->copy(folders[index].path, currentPath);
		
		update(currentPath);
		selectedFolderIndex = copyIndex;
		this->folders[copyIndex].toggleIsSelected();
		this->folders[copyIndex].updateText();
	}
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
	spaceText.setPosition(sf::Vector2f(pos.x + PANEL_MARGIN_X + PANEL_WIDTH - 200, PANEL_HEIGHT + BOTTOM_BUTTONS_HEIGHT / 3));
	spaceText.setCharacterSize(CHARACTER_SIZE + 5);
	spaceText.setFont(fonts[CustomFonts::Font::ROBOTO]);
	window.draw(spaceText);
}

void Panel::updateFolderSelectedFolder(int newSelectedFolder) {
	folders[selectedFolderIndex].toggleIsSelected();
	folders[selectedFolderIndex].updateText();
	selectedFolderIndex = newSelectedFolder;
	folders[selectedFolderIndex].toggleIsSelected();
	folders[selectedFolderIndex].updateText();
}

bool Panel::checkMouseOnFolder(int index, float mouseX, float mouseY) {
	if (!isSelected)
		return false;
	Folder folder = folders[index];
	double extraHeight = 1.f * height / LINE_SPACING;
	return checkBoxLabel(folder.folderText.getPosition().x, folder.folderText.getPosition().y,
		folder.folderText.getPosition().x + PANEL_WIDTH - PANEL_MARGIN_X, folder.folderText.getPosition().y + extraHeight, mouseX, mouseY);
}

bool Panel::checkScrollbarLabel(sf::Vector2f mouse) {
	if (!isSelected)
		return false;
	return checkBoxLabel(scrollbar.getPosition().x, scrollbar.getPosition().y,
		scrollbar.getPosition().x + scrollbar.getSize().x, scrollbar.getPosition().y + scrollbar.getSize().y, mouse.x, mouse.y);
}

void Panel::updateByScrollbar(sf::Vector2f mouse)
{
	if (!isSelected)
		return;
	float x = scrollbar.getPosition().x, y = scrollbar.getPosition().y;
	int steps = (int)std::min(folders.size() - 1.f * lastToDisplay - 1, (mouse.y - y) / scrollbar.getUnitPerFolder());
	if(mouse.y - y < 0)
		steps = -(int)std::min(1.f * firstToDisplay, (y - mouse.y) / scrollbar.getUnitPerFolder());
	updateByScrollbar(steps);
}

void Panel::updateByScrollbar(int steps)
{
	if (!isSelected && lastToDisplay)
		return;
	if (!firstToDisplay && steps < 0)
		return;
	if (lastToDisplay + 1 == folders.size() && steps > 0)
		return;
	lastToDisplay += steps;
	firstToDisplay += steps;
	scrollbar.move(0, steps);
}


void Panel::registerCharacter(int scancode, bool isUpperCase) {
	if (!isSelected || !isSearchActive || isDirectoryLabelActive)
		return;
	if (scancode >= 0 && scancode <= 25) { /// Alphabet
		scancode += 'a';
		if (isUpperCase) scancode -= 'a' - 'A';
		searchText.setString(searchText.getString() + (char)(scancode));
		updateFoldersByFilter();
	}
	else if (scancode == 38) {  /// Delete 
		std::string searchString = searchText.getString().toAnsiString();
		if (searchString.size() > 0)
			searchString.pop_back();
		searchText.setString(searchString);
		updateFoldersByFilter();
	}
	else {
		for (std::pair<int, char> ch : specialCharacters) {
			if (ch.first == scancode) {
				searchText.setString(searchText.getString() + ch.second);
				updateFoldersByFilter();
			}
		}
	}
	
}

void Panel::drawSearchText() {
	std::string searchString = searchText.getString();
	searchString += '|';
	searchText.setString(searchString);
	searchText.setFillColor(textColor);
	searchText.setPosition(sf::Vector2f(pos.x + PANEL_MARGIN_X, PANEL_HEIGHT + BOTTOM_BUTTONS_HEIGHT / 3));
	searchText.setCharacterSize(CHARACTER_SIZE + 5);
	searchText.setFont(fonts[CustomFonts::Font::ROBOTO]);
	window.draw(searchText);
	searchString.pop_back();
	searchText.setString(searchString);
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

void Panel::activateSearch() {
	isSearchActive = true;
	isDirectoryLabelActive = false;
	updateFoldersByFilter();
}

void Panel::deactivateSearch() {
	isSearchActive = false;
	isDirectoryLabelActive = true;
}