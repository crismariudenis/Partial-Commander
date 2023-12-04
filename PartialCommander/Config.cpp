#include "Config.h"

CustomFonts fontsHandler;
std::filesystem::path leftPanelDirectoryPath("D:");
std::filesystem::path rightPanelDirectoryPath("C:");

sf::Vector2f leftPanelTopLeft(10, TOP_BUTTONS_HEIGHT);
sf::Vector2f leftPanelBotRight(WINDOW_WIDTH / 2, WINDOW_HEIGHT - BOTTOM_BUTTONS_HEIGHT);
sf::Vector2f rightPanelTopLeft(WINDOW_WIDTH / 2 + PANEL_OFFSET, TOP_BUTTONS_HEIGHT);
sf::Vector2f rightPanelBotRight(WINDOW_WIDTH, WINDOW_HEIGHT - BOTTOM_BUTTONS_HEIGHT);