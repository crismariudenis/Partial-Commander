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

CustomFonts::CustomFonts() {
    TimesNewRoman.loadFromFile("Fonts/times new roman.ttf");
    Arial.loadFromFile("Fonts/ARIAL.ttf");
    Helvetica.loadFromFile("Fonts/Helvetica.ttf");
    CourierNew.loadFromFile("Fonts/cour.ttf");
}

const std::vector<sf::Font> CustomFonts::getFonts() {
    const std::vector<sf::Font> fonts = { TimesNewRoman, Arial, Helvetica, CourierNew };
    return fonts;
}