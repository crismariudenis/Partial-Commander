#include "Config.h"
#include "Folder.h"
#include "Scrollbar.h"

CustomFonts fontsHandler;

std::filesystem::path leftPanelDirectoryPath("D:\\");
std::filesystem::path rightPanelDirectoryPath("C:\\");

std::unordered_map<sf::Keyboard::Scancode, bool> pressed, released;
std::unordered_map<int, bool> shortcutSelectedFolder;

std::vector<std::pair<int, char>> specialCharacters = { {41, '-'}, {50, '.'}, {40, ' '} };

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
