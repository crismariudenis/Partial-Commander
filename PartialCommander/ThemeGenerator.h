#pragma once

#include "Config.h"


extern void generateTheme();
extern int currentTheme;
extern void readThemes();
extern void writeThemes();

struct RGBColor {
	int r;
	int g;
	int b;
};

struct HSLColor {
	double h;
	double s;
	double l;
};

extern HSLColor rgb_to_hsl(const RGBColor& rgb);
extern bool are_analogous(const RGBColor& color1, const RGBColor& color2, double threshold);
