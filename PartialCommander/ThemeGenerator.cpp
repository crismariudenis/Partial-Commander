#include "ThemeGenerator.h"

double constrastRatio(int r, int g, int b, int r2, int g2, int b2) {
	double relativeLuminance1 = 0.2126 * r + 0.7152 * g + 0.0722 * b;
	double relativeLuminance2 = 0.2126 * r2 + 0.7152 * g2 + 0.0722 * b2;
	double lightColor = std::max(relativeLuminance1, relativeLuminance2), darkColor = std::min(relativeLuminance1, relativeLuminance2);
	return (lightColor + 0.05) / (darkColor + 0.05);
}

const double constrast = 10.00;

void generateTheme()
{
	srand(time(0));
	for (unsigned int index = 0; index < 4; ++index) {
		int red = rand() % 255 + 1, green = rand() % 255 + 1, blue = rand() % 255 + 1;
		if (index > 0) {
			while (!are_analogous({red, green, blue}, { (short)colors[index - 1]->r,  (short)colors[index - 1]->g,  (short)colors[index - 1]->b }, 45.00) && constrastRatio(red, green, blue, (short)(colors[index - 1]->r), (short)colors[index - 1]->g, (short)colors[index - 1]->b) <= constrast)
				red = rand() % 255 + 1, green = rand() % 255 + 1, blue = rand() % 255 + 1;
		}
		*colors[index] = sf::Color(red, green, blue);
	}
	int red = rand() % 255 + 1, green = rand() % 255 + 1, blue = rand() % 255 + 1;
	while (constrastRatio(red, green, blue, (short)colors[0]->r, (short)colors[0]->g, (short)colors[0]->b) <= constrast && constrastRatio(red, green, blue, (short)colors[3]->r, (short)colors[3]->g, (short)colors[3]->b) <= constrast)
		red = rand() % 255 + 1, green = rand() % 255 + 1, blue = rand() % 255 + 1;
	*colors[4] = sf::Color(red, green, blue);
	while (constrastRatio(red, green, blue, (short)colors[0]->r, (short)colors[0]->g, (short)colors[0]->b) <= constrast)
		red = rand() % 255 + 1, green = rand() % 255 + 1, blue = rand() % 255 + 1;
	*colors[5] = sf::Color(red, green, blue);
	while (constrastRatio(red, green, blue, (short)colors[1]->r, (short)colors[1]->g, (short)colors[1]->b) <= constrast)
		red = rand() % 255 + 1, green = rand() % 255 + 1, blue = rand() % 255 + 1;
	*colors[6] = sf::Color(red, green, blue);
	while (constrastRatio(red, green, blue, (short)colors[2]->r, (short)colors[2]->g, (short)colors[2]->b) <= constrast)
		red = rand() % 255 + 1, green = rand() % 255 + 1, blue = rand() % 255 + 1;
	*colors[7] = sf::Color(red, green, blue);
	for (unsigned int index = 0; index < 4; ++index) {
		int red = rand() % 255 + 1, green = rand() % 255 + 1, blue = rand() % 255 + 1;
		*colors[index + 8] = sf::Color(red, green, blue);
	}
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

int currentTheme = 0;

HSLColor rgb_to_hsl(const RGBColor& rgb) {
	double r = rgb.r / 255.0;
	double g = rgb.g / 255.0;
	double b = rgb.b / 255.0;

	double maxVal = std::max(std::max(r, g), b);
	double minVal = std::min(std::min(r, g), b);
	double delta = maxVal - minVal;

	double h = 0.0, s = 0.0, l = (maxVal + minVal) / 2.0;

	if (delta != 0.0) {
		s = (l < 0.5) ? (delta / (maxVal + minVal)) : (delta / (2.0 - maxVal - minVal));

		if (maxVal == r) {
			h = (g - b) / delta + ((g < b) ? 6.0 : 0.0);
		}
		else if (maxVal == g) {
			h = (b - r) / delta + 2.0;
		}
		else {
			h = (r - g) / delta + 4.0;
		}

		h /= 6.0;
		if (h < 0.0) h += 1.0; // Adjust for negative hue
	}

	return { h * 360.0, s * 100.0, l * 100.0 };
}

bool are_analogous(const RGBColor& color1, const RGBColor& color2, double threshold = 30.0) {
	HSLColor hsl1 = rgb_to_hsl(color1);
	HSLColor hsl2 = rgb_to_hsl(color2);

	double hueDifference = std::abs(hsl1.h - hsl2.h);

	// Ensure the hue difference considers the circular nature of the color wheel
	if (hueDifference > 180.0) {
		hueDifference = 360.0 - hueDifference;
	}

	return hueDifference <= threshold;
}