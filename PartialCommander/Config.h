#include <SFML/Graphics.hpp>
#include "CustomFonts.h"
#include <filesystem>
#pragma once

/// Window Constraints
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1500
#define TOP_BUTTONS_HEIGHT 75
#define BOTTOM_BUTTONS_HEIGHT 100

/// Panel Constrains
#define FOLDER_SPACE 400
#define SIZE_SPACE 80 
#define TIME_SPACE 300
#define BORDER_OFFSET 30
#define PANEL_BOTTOM_WIDTH WINDOW_WIDTH / 2
#define PANEL_BOTTOM_HEIGHT 50
#define PANEL_HEIGHT WINDOW_HEIGHT - BOTTOM_BUTTONS_HEIGHT - TOP_BUTTONS_HEIGHT
#define PANEL_OFFSET 30
#define LINE_SPACING 23

#define PANEL_MARGIN_X 10
#define PANEL_MARGIN_TOP 20
#define PANEL_MARGIN_BOTOOM 40 
#define PANEL_LINE_WIDTH 2
#define PANEL_WIDTH (WINDOW_WIDTH - 3 * PANEL_MARGIN_X - 2*PANEL_LINE_WIDTH)/2


extern std::filesystem::path leftPanelDirectoryPath;
extern std::filesystem::path rightPanelDirectoryPath;
  

/// TEXT Constants
#define CHARACTER_SIZE 15
const sf::Color columnColor(199, 192, 149); /// Color for the Columns
const sf::Color backgroundColor(0, 1, 169); /// Background Color 
const sf::Color textColor(181, 183, 229); /// Text Color



/// Fonts
extern CustomFonts fontsHandler;
extern std::vector<sf::Font> fonts;




/// Positions
extern sf::Vector2f leftPanelTopLeft;
extern sf::Vector2f leftPanelBotRight;
extern sf::Vector2f rightPanelTopLeft;
extern sf::Vector2f rightPanelBotRight;

