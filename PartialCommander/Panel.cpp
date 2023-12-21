#include "Panel.h"


void Panel::init(sf::Vector2f pos, int width, int height, std::filesystem::path currentPath, std::vector<sf::Font>& fonts) {
	this->pos = pos;
	this->width = width;
	this->height = height;
	this->currentPath = currentPath;
	this->fonts = fonts;
	update(currentPath);
}

void Panel::drawFolders() {
	for (int index = firstToDisplay; index <= lastToDisplay; ++index) {
		window.draw(folders[index].folderText);
		window.draw(folders[index].sizeText);
		window.draw(folders[index].dateText);
	}
}
void Panel::drawBorders() {

	sf::RectangleShape rect;
	rect.setOutlineThickness(PANEL_LINE_WIDTH);
	rect.setPosition(pos);
	rect.setSize(sf::Vector2f(width, height));
	rect.setOutlineColor(sf::Color::White);
	rect.setFillColor(sf::Color::Transparent);

	sf::RectangleShape line1;
	line1.setOutlineThickness(PANEL_LINE_WIDTH / 2);
	line1.setPosition(pos + sf::Vector2f(FOLDER_SPACE, 0));
	line1.setSize(sf::Vector2f(0, height - PANEL_BOTTOM_HEIGHT));
	line1.setOutlineColor(sf::Color::White);

	sf::RectangleShape line2;
	line2.setOutlineThickness(PANEL_LINE_WIDTH / 2);
	line2.setPosition(pos + sf::Vector2f(FOLDER_SPACE + SIZE_SPACE, 0));
	line2.setSize(sf::Vector2f(0, height - PANEL_BOTTOM_HEIGHT));
	line2.setOutlineColor(sf::Color::White
	);

	sf::RectangleShape line3;
	line3.setOutlineThickness(PANEL_LINE_WIDTH / 2);
	line3.setPosition(pos + sf::Vector2f(0, PANEL_HEIGHT - PANEL_BOTTOM_HEIGHT));
	line3.setSize(sf::Vector2f(width, 0));
	line3.setOutlineColor(sf::Color::White);


	window.draw(rect);
	window.draw(line1);
	window.draw(line2);
	window.draw(line3);
}

void Panel::drawColumnTitles() {

	sf::Text folderName;

	folderName.setString("Name");

	sf::FloatRect rc = folderName.getLocalBounds();
	folderName.setCharacterSize(CHARACTER_SIZE);
	folderName.setFillColor(titleColor);
	folderName.setOrigin(rc.width / 2, 0);
	folderName.setFont(fonts[CustomFonts::Font::ROBOTO]);
	folderName.setPosition(pos + sf::Vector2f(FOLDER_SPACE / 2, 10));

	window.draw(folderName);

	sf::Text sizeName = folderName;
	sizeName.setString("Size");


	rc = sizeName.getLocalBounds();
	sizeName.setOrigin(rc.width / 2, 0);
	sizeName.setPosition(pos + sf::Vector2f(FOLDER_SPACE + SIZE_SPACE / 2, 10));
	window.draw(sizeName);

	sf::Text modifyName = sizeName;
	modifyName.setString("Modify Time");

	rc = modifyName.getLocalBounds();
	modifyName.setOrigin(rc.width / 2, 0);
	modifyName.setPosition(pos + sf::Vector2f(FOLDER_SPACE + SIZE_SPACE + TIME_SPACE / 2, 10));
	window.draw(modifyName);

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
	if (isSelected)
		drawSelectedFolderBackground();
	drawFolders();
	drawBorders();
	drawColumnTitles();
	drawCurrentPath();
	drawFreeSpace();
}
void Panel::update(std::filesystem::path path) {

	if (!std::filesystem::exists(path))
		return;

	this->currentPath = path;
	sf::Vector2f textPosition = pos;
	textPosition.x += 10;
	textPosition.y += 1.0 * height / LINE_SPACING + TOP_TEXT_BORDER;
	firstToDisplay = 0, lastToDisplay = 1, selectedFolderIndex = 0;

	folders.clear();
	folders.push_back(Folder("/..", textPosition, fonts, " "));
	folders[firstToDisplay].toggleIsSelected();

	for (auto const& entry : std::filesystem::directory_iterator(path)) {
		textPosition.y += height / LINE_SPACING;
		folders.push_back(Folder(entry.path(), textPosition, fonts, getDate(entry.path())));
		if (textPosition.y <= height - PANEL_OFFSET - 20) {
			lastToDisplay++;
		}
	}
	lastToDisplay--;

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

	if (isSelected) {
		switch (code)
		{
		case sf::Keyboard::Scancode::S:
		case sf::Keyboard::Scancode::Down:
		{
			if (selectedFolderIndex + 1 < folders.size()) {
				folders[selectedFolderIndex].toggleIsSelected();
				folders[selectedFolderIndex].updateText();
				selectedFolderIndex++;
				folders[selectedFolderIndex].toggleIsSelected();
				folders[selectedFolderIndex].updateText();
			}
			if (selectedFolderIndex == lastToDisplay) {
				int step = std::min((size_t)10, folders.size() - lastToDisplay - 1);
				firstToDisplay += step;
				lastToDisplay += step;
				updateFoldersPosition(sf::Vector2f(0, 1.f * step * (-height) / LINE_SPACING));
			}
			break;
		}
		case sf::Keyboard::Scancode::W:
		case sf::Keyboard::Scancode::Up:
		{
			if (selectedFolderIndex != 0) {
				folders[selectedFolderIndex].toggleIsSelected();
				folders[selectedFolderIndex].updateText();
				selectedFolderIndex--;
				folders[selectedFolderIndex].toggleIsSelected();
				folders[selectedFolderIndex].updateText();
			}
			if (selectedFolderIndex == firstToDisplay) {
				int step = std::min(10, firstToDisplay);
				firstToDisplay -= step;
				lastToDisplay -= step;
				updateFoldersPosition(sf::Vector2f(0, 1.f * step * height / LINE_SPACING));
			}
			break;
		}

		case sf::Keyboard::Scancode::F8:
		{
			int index = selectedFolderIndex;
			auto path = folders[index].path;

			if (std::filesystem::is_directory(path))
				std::cout << "Folder: " << path << " removed\n";
			else
				std::cout << "File: " << path << " removed\n";

			// remove from the filesystem
			// if it's a folder remove recursively 
			std::filesystem::remove_all(path);

			update(currentPath);
			//update the index to the old one
			selectedFolderIndex = std::min(index, (int)folders.size() - 1);
			break;
		}
		case sf::Keyboard::Scancode::F5: {

			// Todo: Choose the folder in which the file is coppied
			int index = selectedFolderIndex;
			auto path = folders[index].path;
			auto destPath = path;

			std::string extension = path.extension().string();
			for (int i = 1; std::filesystem::exists(destPath); i++) {
				destPath = path.parent_path() / (path.stem().string() + " (" + std::to_string(i) + ")" + extension);
			}

			try {
				std::filesystem::copy(path, destPath, std::filesystem::copy_options::recursive);
				std::cout << destPath.filename() << " coppied to " << destPath.parent_path() << '\n';
			}
			catch (const std::filesystem::filesystem_error& e) {
				std::cerr << "Error copying file: " << e.what() << std::endl;
			}
			update(path.parent_path());

			// reset the index
			selectedFolderIndex = index;
			break;
		}
		default:
			break;
		}
	}
}

void Panel::updateFoldersPosition(sf::Vector2f move) {
	for (int index = 0; index < folders.size(); ++index) {
		sf::Vector2f folderPosition = folders[index].position;
		folderPosition += move;
		folders[index].position = folderPosition;
		folders[index].updateText();
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

void Panel::drawCurrentPath()
{
	sf::Text pathText;
	pathText.setString(currentPath.string());
	pathText.setFillColor(textColor);
	pathText.setPosition(sf::Vector2f(pos.x + PANEL_MARGIN_X, PANEL_HEIGHT + BOTTOM_BUTTONS_HEIGHT / 3));
	pathText.setCharacterSize(CHARACTER_SIZE + 5);
	pathText.setFont(fonts[CustomFonts::Font::ROBOTO]);
	window.draw(pathText);
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

	auto initPositions = [](std::vector<Folder>& folders) {
		std::vector<sf::Vector2f> initialPositions;
		for (int index = 0; index < folders.size(); ++index)
			initialPositions.push_back(folders[index].position);
		return initialPositions;
		};

	std::vector<sf::Vector2f> initialPositions;

	if (checkBoxLabel(pos.x, pos.y, pos.x + FOLDER_SPACE, pos.y + 20.f, mouse.x, mouse.y)) {
		initialPositions = initPositions(folders);
		folders[selectedFolderIndex].toggleIsSelected();
		if (abs(sortType) == 1) {
			reverse(folders.begin() + 1, folders.end());
			sortType = -sortType;
		}
		else std::sort(folders.begin() + 1, folders.end(), nameCompare), sortType = 1;
		resetTextPositions(initialPositions);
	}
	else if (checkBoxLabel(pos.x + FOLDER_SPACE + 1, pos.y, pos.x + FOLDER_SPACE + SIZE_SPACE, pos.y + 20, mouse.x, mouse.y)) {
		initialPositions = initPositions(folders);
		folders[selectedFolderIndex].toggleIsSelected();
		if (abs(sortType) == 2) {
			reverse(folders.begin() + 1, folders.end());
			sortType = -sortType;
		}
		else std::sort(folders.begin() + 1, folders.end(), sizeCompare), sortType = 2;
		resetTextPositions(initialPositions);
	}
	else if (checkBoxLabel(pos.x + FOLDER_SPACE + SIZE_SPACE + 1, pos.y, pos.x + width, pos.y + 20, mouse.x, mouse.y)) {
		initialPositions = initPositions(folders);
		folders[selectedFolderIndex].toggleIsSelected();
		if (abs(sortType) == 3) {
			reverse(folders.begin() + 1, folders.end());
			sortType = -sortType;
		}
		else std::sort(folders.begin() + 1, folders.end(), timeCompare), sortType = 3;
		resetTextPositions(initialPositions);
	}
}

void Panel::resetTextPositions(std::vector<sf::Vector2f> initialPositions) {
	for (int index = 0; index < folders.size(); ++index) {
		folders[index].position = initialPositions[index];
		if (index == selectedFolderIndex)
			folders[index].toggleIsSelected();
		folders[index].updateText();
	}
}

void Panel::activateLabel(int mouseX, int mouseY) {
	if (checkBoxLabel(pos.x, pos.y, pos.x + FOLDER_SPACE, pos.y + 20, mouseX, mouseY)) {
		sf::RectangleShape background;
		initBackground(background, sf::Color(68, 85, 90), pos, sf::Vector2f(FOLDER_SPACE, 30));
		window.draw(background);
	}
	else if (checkBoxLabel(pos.x, pos.y, pos.x + FOLDER_SPACE + SIZE_SPACE, pos.y + 20, mouseX, mouseY)) {
		sf::RectangleShape background;
		initBackground(background, sf::Color(68, 85, 90), sf::Vector2f(pos.x + FOLDER_SPACE, pos.y), sf::Vector2f(SIZE_SPACE, 30));
		window.draw(background);
	}
	else if (checkBoxLabel(pos.x + FOLDER_SPACE + SIZE_SPACE, pos.y, pos.x + width, pos.y + 20, mouseX, mouseY)) {
		sf::RectangleShape background;
		initBackground(background, sf::Color(68, 85, 90), sf::Vector2f(pos.x + FOLDER_SPACE + SIZE_SPACE, pos.y), sf::Vector2f(TIME_SPACE, 30));
		window.draw(background);
	}
}

void Panel::resetFoldersPositions() {
	sf::Vector2f textPosition = sf::Vector2f(pos.x, pos.y + TOP_TEXT_BORDER);
	textPosition.x += 10;
	textPosition.y += 1.f * height / LINE_SPACING;
	for (int index = 0; index < folders.size(); ++index) {
		folders[index].position = textPosition;
		textPosition.y += 1.f * height / LINE_SPACING;
	}
}

int Panel::getSelectedFolderIndex() {
	return selectedFolderIndex;
}

void Panel::addFolder(Folder folder, int position) {
	folders.insert(folders.begin() + position, folder);
}

void Panel::updateShortcutSelectedFolder(int type, int move)
{
	if (!isSelected)
		return;
	shortcutSelectedFolder[selectedFolderIndex] = true;
	if (type == 1) { /// Ctrl A
		for (int i = 0; i < folders.size(); ++i)
			shortcutSelectedFolder[i] = true;
	}
	else if (type == 2) { /// Ctrl Shift Up/Down
		if (shiftSelectedFolder - selectedFolderIndex > 0) {
			if (move > 0) shortcutSelectedFolder[++shiftSelectedFolder] = true;
			else shortcutSelectedFolder[shiftSelectedFolder--] = false;
		}
		else if (shiftSelectedFolder - selectedFolderIndex < 0) {
			if (move > 0) shortcutSelectedFolder[shiftSelectedFolder++] = false;
			else shortcutSelectedFolder[--shiftSelectedFolder] = true;
		}
		else {
			if (move > 0) shortcutSelectedFolder[++shiftSelectedFolder] = true;
			else shortcutSelectedFolder[--shiftSelectedFolder] = true;
		}
	}
	else if (type == 3) {
		shortcutSelectedFolder.clear();
		shiftSelectedFolder = selectedFolderIndex;
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

		for (size_t index = 0; index < folders.size(); ++index) {
			std::string path = folders[index].path.string(), suffix;
			while (path.back() != '\\')
				suffix += path.back(), path.pop_back();
			std::reverse(suffix.begin(), suffix.end());
			std::string destPath = currentPath.string() + '\\' + suffix;
			for (int i = 1; std::filesystem::exists(destPath); i++) {
				destPath = currentPath.string() + '\\' + suffix + std::to_string(i);
			}
			std::cout << destPath << '\n';
			std::filesystem::copy(path, destPath, std::filesystem::copy_options::recursive);
		}
		update(currentPath);
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