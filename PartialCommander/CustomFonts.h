#include <SFML/Graphics.hpp>

#pragma once

class CustomFonts
{
private:
    sf::Font TimesNewRoman;
    sf::Font Arial;
    sf::Font Helvetica;
    sf::Font CourierNew;

public:
    CustomFonts();

    const std::vector<sf::Font> getFonts();

    sf::Font getTimesNewRoman();
    sf::Font getArial();
    sf::Font getHelvetica();
    sf::Font getCourierNew();

};