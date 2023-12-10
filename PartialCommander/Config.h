#include <SFML/Graphics.hpp>
#include "CustomFonts.h"
#include <filesystem>
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <map>


#pragma once

/// Window Constraints
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1500
#define TOP_BUTTONS_HEIGHT 50
#define BOTTOM_BUTTONS_HEIGHT 100

/// Panel Constrains
#define FOLDER_SPACE 350
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
#define BUTTON_HEIGHT 20

extern std::filesystem::path leftPanelDirectoryPath;
extern std::filesystem::path rightPanelDirectoryPath;


/// TEXT Constraints
#define CHARACTER_SIZE 15
const sf::Color columnColor(199, 192, 149); /// Color for the Columns
const sf::Color backgroundColor(19, 103, 208); /// Background Color 
const sf::Color textColor = sf::Color::White; /// Text Color
const sf::Color titleColor = sf::Color(215, 171, 32);
const sf::Color secondaryColor(42, 161, 179); /// Buttons and Selected Folder Color
const sf::Color selectedTextColor(0, 0, 0); /// Selected Folder Text Color
const sf::Color bottomBackgroundColor(0, 0, 0);
const std::vector<std::string> buttonNames = { "Help", "Menu", "View", "Edit", "Copy", "RenMov", "Mkdir", "Delete", "PullDn", "Quit" };

/// Fonts
extern CustomFonts fontsHandler;

inline void initBackground(sf::RectangleShape& background, sf::Color color, sf::Vector2f position, sf::Vector2f size) {
	background.setFillColor(color);
	background.setPosition(position);
	background.setSize(size);
}




