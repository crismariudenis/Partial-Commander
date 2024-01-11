#include "Menu.h"

void Menu::init() {
	if (!t1.loadFromFile(image1)) {
		std::cerr << "Failed to load image " << image1 << '\n';
		return;
	}
	if (!t2.loadFromFile(image2)) {
		std::cerr << "Failed to load image " << image2 << '\n';
		return;
	}
	s1.setTexture(t1);
	s2.setTexture(t2);

	s1.setScale(sf::Vector2f{ 0.4,0.4 });
	s2.setScale(sf::Vector2f{ 0.4,0.4 });

	s1.setPosition(pos + sf::Vector2f{ MENU_INNER_PADDING_X,MENU_INNER_PADDING_Y });
	s2.setPosition(pos + sf::Vector2f{ MENU_WIDTH,0 } - sf::Vector2f{ s1.getGlobalBounds().width + MENU_INNER_PADDING_X,-MENU_INNER_PADDING_Y });

	background.setFillColor(sf::Color(208, 207, 204));
	background.setPosition(pos);
	background.setSize(sf::Vector2f{ MENU_WIDTH,MENU_HEIGHT });

	darkBackGround.setFillColor(sf::Color(0, 0, 0));
	darkBackGround.setPosition(pos + sf::Vector2f{ 20,20 });
	darkBackGround.setSize(sf::Vector2f{ MENU_WIDTH,MENU_HEIGHT });

	fonts = fontsHandler.getFonts();
	text.setFillColor(sf::Color(233, 172, 19));
	text.setFont(fonts[CustomFonts::Font::UBUNTU]);
	text.setCharacterSize(MENU_CHARACTER_SIZE);

	text.setString("The Authors");

	sf::FloatRect rc = text.getLocalBounds();
	text.setOrigin(rc.width / 2, 0);

	text.setPosition(pos + sf::Vector2f(MENU_WIDTH / 2, s1.getGlobalBounds().height / 2));

	name1.setString("Denis Crismariu");
	name1.setFillColor(sf::Color(233, 172, 19));
	name1.setCharacterSize(MENU_NAME_CHARACTER_SIZE);
	name1.setFont(fonts[CustomFonts::Font::UBUNTU]);
	name2 = name1;

	name2.setString("Stefan Ciornei");

	rc = name1.getLocalBounds();
	name1.setPosition(s1.getPosition() + sf::Vector2f(s1.getGlobalBounds().width / 2, s1.getGlobalBounds().height + MENU_INNER_PADDING_Y));
	name1.setOrigin(rc.width / 2, rc.height / 2);

	rc = name2.getLocalBounds();
	name2.setPosition(s2.getPosition() + sf::Vector2f(s2.getGlobalBounds().width / 2, s2.getGlobalBounds().height + MENU_INNER_PADDING_Y));
	name2.setOrigin(rc.width / 2, rc.height / 2);

	boxY1 = s1.getPosition().y;
	boxY2 = name1.getGlobalBounds().height + name1.getPosition().y;

}
void Menu::draw() {
	window.draw(darkBackGround);
	window.draw(background);

	window.draw(s1);
	window.draw(s2);

	window.draw(name1);
	name1.setPosition(name1.getPosition());
	window.draw(name2);

	window.draw(text);
	fps++;
	if (fps % 30 == 0)
		gravity();
}
void Menu::gravity() {

	name1.setPosition(name1.getPosition() + v);
	name2.setPosition(name2.getPosition() + v);
	s1.setPosition(s1.getPosition() + v);
	s2.setPosition(s2.getPosition() + v);

	boxY1 += v.y, boxY2 += v.y;

	v += g;
	if (boxY2 + v.y + 5 > pos.y + MENU_HEIGHT)
			v.y = -v.y * 0.7;
}