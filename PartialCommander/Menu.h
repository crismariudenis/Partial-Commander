#pragma once

#include "Config.h"

class Menu {
	sf::RenderWindow& window;
	sf::Texture t1, t2;
	sf::Sprite s1, s2;
	std::vector<sf::Font> fonts;
	sf::Vector2f g{ 0,1 };
	const sf::Vector2f pos{ MENU_PADDING_X,MENU_PADDING_Y };
	float boxY1, boxY2;
	sf::RectangleShape background,darkBackGround;
	sf::Text text,name1,name2;
	std::string image1{ "Assets/denis1.jpg" };
	std::string image2{ "Assets/ciornei1.jpg" };
	void gravity();
public:
	Menu(sf::RenderWindow& window) :window(window) {};
	void init();
	void draw();
};