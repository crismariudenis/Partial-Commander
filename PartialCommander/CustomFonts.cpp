#include "CustomFonts.h"


sf::Font CustomFonts::getTimesNewRoman() {
	return TimesNewRoman;
}

sf::Font CustomFonts::getArial() {
	return Arial;
}

sf::Font CustomFonts::getHelvetica() {
	return Helvetica;
}

sf::Font CustomFonts::getCourierNew() {
	return CourierNew;
}

sf::Font CustomFonts::getRoboto() {
	return Roboto;
}
sf::Font CustomFonts::getUbuntu() {
	return Ubuntu;
}

CustomFonts::CustomFonts() {
	TimesNewRoman.loadFromFile("Fonts/times new roman.ttf");
	Arial.loadFromFile("Fonts/ARIAL.ttf");
	Helvetica.loadFromFile("Fonts/Helvetica.ttf");
	CourierNew.loadFromFile("Fonts/cour.ttf");
	Roboto.loadFromFile("Fonts/RobotoMono-Bold.ttf");
	Ubuntu.loadFromFile("Fonts/UbuntuMono-Bold.ttf");
}

std::vector<sf::Font> CustomFonts::getFonts() {
	const std::vector<sf::Font> fonts = { TimesNewRoman, Arial, Helvetica, CourierNew, Roboto ,Ubuntu};
	return fonts;
}