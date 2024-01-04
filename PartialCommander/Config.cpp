#include "Config.h"
#include "Folder.h"
#include "Scrollbar.h"

CustomFonts fontsHandler;

std::filesystem::path leftPanelDirectoryPath("D:\\");
std::filesystem::path rightPanelDirectoryPath("C:\\");

std::unordered_map<int, bool> shortcutSelectedFolder, pressed;
std::vector<std::pair<int, char>> specialCharacters = { {41, '-'}, {50, '.'}, {40, ' '} , {45, '\\'} };


bool checkBoxLabel(float topLeftX, float topLeftY, float botRightX, float botRightY, float mouseX, float mouseY)
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

void generateTheme() 
{
	srand(time(0));
	for (int index = 0; index < colors.size(); ++index) {
		int red = rand() % 255 + 1, green = rand() % 255 + 1, blue = rand() % 255 + 1, transparency = rand() % 255 + 1;
		*colors[index] = sf::Color(red, green, blue, transparency);
	}
}

sf::Color columnColor(199, 192, 149); /// Color for the Columns
sf::Color backgroundColor(19, 103, 208); /// Background Color 
sf::Color textColor(255, 255, 255); /// Text Color
sf::Color titleColor = sf::Color(215, 171, 32);
sf::Color secondaryColor(42, 161, 179); /// Buttons and Selected Folder Color
sf::Color selectedTextColor(0, 0, 0); /// Selected Folder Text Color
sf::Color bottomBackgroundColor(0, 0, 0);
sf::Color scrollbarButtonColor(198, 215, 251, 255);
sf::Color scrollbarTextButtonColor(81, 96, 138, 255);
sf::Color scrollbarColor(198, 215, 251, 255);
sf::Color sortLabelColor(3, 75, 166);
sf::Color outlineColor(255, 255, 153);

std::vector<sf::Color*> colors = { &columnColor, &backgroundColor, &textColor, &titleColor, &secondaryColor, &selectedTextColor, &bottomBackgroundColor, &scrollbarButtonColor, &scrollbarTextButtonColor, &scrollbarColor, &sortLabelColor, &outlineColor};

