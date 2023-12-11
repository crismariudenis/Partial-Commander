#include "Config.h"

CustomFonts fontsHandler;
std::filesystem::path leftPanelDirectoryPath("D:\\");
std::filesystem::path rightPanelDirectoryPath("C:\\");

sf::Vector2f leftPanelTopLeft(10, TOP_BUTTONS_HEIGHT);
sf::Vector2f leftPanelBotRight(WINDOW_WIDTH / 2, WINDOW_HEIGHT - BOTTOM_BUTTONS_HEIGHT);
sf::Vector2f rightPanelTopLeft(WINDOW_WIDTH / 2 + PANEL_OFFSET, TOP_BUTTONS_HEIGHT);
sf::Vector2f rightPanelBotRight(WINDOW_WIDTH, WINDOW_HEIGHT - BOTTOM_BUTTONS_HEIGHT);

std::map<std::string, std::string> dateName = { {"01", "Jan"}, {"02", "Feb"}, {"03", "Mar"}, {"04", "Apr"}, {"05", "May"},
												{"06", "Jun"}, {"07", "Jul"}, {"08", "Aug"}, {"09", "Sep"}, {"10", "Oct"},
												{"11", "Nov"}, {"12", "Dec"} };