#include <SFML/Graphics.hpp>
#include "CustomFonts.h"
#include <filesystem>
#include <iostream>
#include <stdio.h>
#include <chrono>  // Add this for chrono
#include <iomanip> // Add this for put_time
#include <sstream> // Add this for stringstream
#include <windows.h>
#include <map>
#include <unordered_map>
#include <cmath>
#include "fstream"

#pragma once

class Folder;

/// Window Constraints
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1500
#define TOP_BUTTONS_HEIGHT 0
#define BOTTOM_BUTTONS_HEIGHT 100

/// Panel Constrains
#define BORDER_OFFSET 30
#define PANEL_BOTTOM_WIDTH (WINDOW_WIDTH / 2)
#define PANEL_BOTTOM_HEIGHT 50
#define PANEL_HEIGHT (WINDOW_HEIGHT - BOTTOM_BUTTONS_HEIGHT - TOP_BUTTONS_HEIGHT)
#define PANEL_OFFSET 30
#define LINE_SPACING 23
#define TEXT_LINE_SPACING 30


#define MAX_FOLDERS_NUMBER 300

#define PANEL_MARGIN_X 10
#define PANEL_MARGIN_TOP 20
#define PANEL_MARGIN_BOTOOM 40 
#define PANEL_LINE_WIDTH 2
#define PANEL_WIDTH (WINDOW_WIDTH - 3 * PANEL_MARGIN_X - 2*PANEL_LINE_WIDTH)/2
#define BUTTON_HEIGHT 20

#define FOLDER_SPACE 530
#define SIZE_SPACE 80 
#define TIME_SPACE (PANEL_WIDTH - SIZE_SPACE - FOLDER_SPACE)

#define SCROLLBAR_WIDTH 10
#define SCROLLBAR_X (PANEL_WIDTH + 12)
#define SCROLLBAR_Y (TOP_BUTTONS_HEIGHT + 18)
#define SCROLLBAR_BUTTON_WIDTH SCROLLBAR_WIDTH
#define SCROLLBAR_BUTTON_HEIGHT 20

#define EDITOR_PADDING 10
#define EDITOR_MAX_LINES 32
#define CLICK_ERROR 500

// Menu
#define MENU_CHARACTER_SIZE 40
#define MENU_NAME_CHARACTER_SIZE 30
#define MENU_PADDING_X 300
#define MENU_PADDING_Y 60
#define MENU_INNER_PADDING_X 40
#define MENU_INNER_PADDING_Y 30
#define MENU_WIDTH (WINDOW_WIDTH - 2 * MENU_PADDING_X)
#define MENU_HEIGHT (WINDOW_HEIGHT - BOTTOM_BUTTONS_HEIGHT - 2 * MENU_PADDING_Y)

extern std::filesystem::path leftPanelDirectoryPath;
extern std::filesystem::path rightPanelDirectoryPath;


/// TEXT Constraints
#define CHARACTER_SIZE 15
#define TOP_TEXT_BORDER 10


extern sf::Color columnColor; /// Color for the Columns
extern sf::Color backgroundColor; /// Background Color 
extern sf::Color textColor; /// Text Color
extern sf::Color titleColor;
extern sf::Color secondaryColor; /// Buttons and Selected Folder Color
extern sf::Color selectedTextColor; /// Selected Folder Text Color
extern sf::Color bottomBackgroundColor;
extern sf::Color scrollbarButtonColor;
extern sf::Color scrollbarTextButtonColor;
extern sf::Color scrollbarColor;
extern sf::Color sortLabelColor;
extern sf::Color outlineColor;

extern std::vector<sf::Color*> colors;

const std::vector<std::string> buttonNames = { "Credits", "Rename", "View", "Edit", "Copy", "Paste", "Mkdir", "Delete", "Save", "Quit" };
//const std::vector<std::string> editorButtonNames = {}

/// Fonts
extern CustomFonts fontsHandler;

inline void initBackground(sf::RectangleShape& background, sf::Color color, sf::Vector2f position, sf::Vector2f size) {
	background.setFillColor(color);
	background.setPosition(position);
	background.setSize(size);
}

extern std::unordered_map<int, int> pressed;
extern std::vector<std::pair<int, char>> specialCharacters;
extern std::unordered_map<int, bool> shortcutSelectedFolder;
extern std::vector<sf::Color> themes[10];

extern bool checkBoxLabel(float topLeftX, float topLeftY, float botRightX, float botRightY, float mouseX, float mouseY);
extern bool nameCompare(const Folder& a, const Folder& b);
extern bool sizeCompare(const Folder& a, const Folder& b);
extern bool timeCompare(const Folder& a, const Folder& b);
extern void generateTheme();
extern int pressedKeys;