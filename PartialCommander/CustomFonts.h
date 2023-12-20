#include <SFML/Graphics.hpp>

#pragma once

class CustomFonts
{
private:
	sf::Font TimesNewRoman;
	sf::Font Arial;
	sf::Font Helvetica;
	sf::Font CourierNew;
	sf::Font Roboto;


public:
	enum Font {
		TIMES_NEW_ROMAN,
		ARIAL,
		HELVETICA,
		COURIER_NEW,
		ROBOTO
	};
	CustomFonts();

	const std::vector<sf::Font> getFonts();

	sf::Font getTimesNewRoman();
	sf::Font getArial();
	sf::Font getHelvetica();
	sf::Font getCourierNew();
	sf::Font getRoboto();

};