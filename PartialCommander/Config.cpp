#include "Config.h"

CustomFonts fontsHandler;

std::filesystem::path leftPanelDirectoryPath("D:\\");
std::filesystem::path rightPanelDirectoryPath("C:\\Users\\Denis\\Downloads");

std::unordered_map<sf::Keyboard::Scancode, bool> pressed, released;

std::vector<std::pair<int, char>> specialCharacters = { {41, '-'}, {50, '.'}, {40, ' '} };