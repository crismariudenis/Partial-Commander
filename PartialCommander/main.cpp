#include "App.h"

void writeThemes();
void readThemes();

int main()
{
    App app{"Partial Commander"};
    app.init();
    readThemes();
    app.run();
    writeThemes();
    return 0;
}

void writeThemes() {
    std::ofstream g("Themes/Themes.txt");
    for (int index = 0; index < 10; ++index) {
        for (int j = 0; j < themes[index].size(); ++j) {
            int code = themes[index][j].toInteger();
            g << ((code >> 24) & 255) << ' ' << ((code >> 16) & 255) << ' ' << ((code >> 8) & 255) << ' ';
        }
        g << '\n';
    }
    g.close();
}

void readThemes() {
    std::ifstream f("Themes/Themes.txt");
    std::string themeLine;
    int currIndex = 0;
    while (std::getline(f, themeLine)) {
        for (int index = 0; index < themeLine.size(); index++) {
            int red = 0, blue = 0, green = 0;
            while (index < themeLine.size() && themeLine[index] != ' ')
                red = red * 10 + themeLine[index++] - '0';
            index++;
            while (index < themeLine.size() && themeLine[index] != ' ')
                green = green * 10 + themeLine[index++] - '0';
            index++;
            while (index < themeLine.size() && themeLine[index] != ' ')
                blue = blue * 10 + themeLine[index++] - '0';
            themes[currIndex].push_back(sf::Color(red, green, blue));
        
        }
        currIndex++;
    }
    f.close();
}