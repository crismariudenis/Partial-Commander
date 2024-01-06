#include "Config.h"
#include "Folder.h"
#include "Scrollbar.h"

CustomFonts fontsHandler;

std::filesystem::path leftPanelDirectoryPath("D:\\");
std::filesystem::path rightPanelDirectoryPath("C:\\");

std::unordered_map<int, bool> shortcutSelectedFolder;
std::unordered_map<int, int> pressed;
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

double constrastRatio(int r, int g, int b, int r2, int g2, int b2) {
	double relativeLuminance1 = 0.2126 * r + 0.7152 * g + 0.0722 * b;
	double relativeLuminance2 = 0.2126 * r2 + 0.7152 * g2 + 0.0722 * b2;
	double lightColor = std::max(relativeLuminance1, relativeLuminance2), darkColor = std::min(relativeLuminance1, relativeLuminance2);
	return (lightColor + 0.05) / (darkColor + 0.05);
}

const double constrast = 6.00;

void generateTheme()
{
	srand(time(0));
	for (unsigned int index = 0; index < 4; ++index) {
		int red = rand() % 255 + 1, green = rand() % 255 + 1, blue = rand() % 255 + 1;
		if (index > 0) {
			while (constrastRatio(red, green, blue, (short)(colors[index - 1]->r), (short)colors[index - 1]->g, (short)colors[index - 1]->b) <= constrast) {
				red = rand() % 255 + 1, green = rand() % 255 + 1, blue = rand() % 255 + 1;
			}
		}
		*colors[index] = sf::Color(red, green, blue);
	}
	int red = rand() % 255 + 1, green = rand() % 255 + 1, blue = rand() % 255 + 1;
	while (constrastRatio(red, green, blue, (short)colors[0]->r, (short)colors[0]->g, (short)colors[0]->b) <= constrast && constrastRatio(red, green, blue, (short)colors[3]->r, (short)colors[3]->g, (short)colors[3]->b) <= constrast) 
		red = rand() % 255 + 1, green = rand() % 255 + 1, blue = rand() % 255 + 1;
	*colors[4] = sf::Color(red, green, blue);
	while (constrastRatio(red, green, blue, (short)colors[0]->r, (short)colors[0]->g, (short)colors[0]->b) <= constrast)
		red = rand() % 255 + 1, green = rand() % 255 + 1, blue = rand() % 255 + 1;
	*colors[5] = sf::Color(red, green, blue);
	while (constrastRatio(red, green, blue, (short)colors[1]->r, (short)colors[1]->g, (short)colors[1]->b) <= constrast)
		red = rand() % 255 + 1, green = rand() % 255 + 1, blue = rand() % 255 + 1;
	*colors[6] = sf::Color(red, green, blue);
	while (constrastRatio(red, green, blue, (short)colors[2]->r, (short)colors[2]->g, (short)colors[2]->b) <= constrast)
		red = rand() % 255 + 1, green = rand() % 255 + 1, blue = rand() % 255 + 1;
	*colors[7] = sf::Color(red, green, blue);
	for (unsigned int index = 0; index < 4; ++index) {
		int red = rand() % 255 + 1, green = rand() % 255 + 1, blue = rand() % 255 + 1;
		*colors[index + 8] = sf::Color(red, green, blue);
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

std::vector<sf::Color*> colors = { &backgroundColor, &bottomBackgroundColor, &secondaryColor, &sortLabelColor, &titleColor, &textColor ,&columnColor,& selectedTextColor,& scrollbarTextButtonColor,& scrollbarColor,& scrollbarButtonColor,& outlineColor };
std::vector<sf::Color> themes[10];
int currentTheme = 0, pressedKeys = 0;