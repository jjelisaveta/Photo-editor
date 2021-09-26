#include <iostream>
#include "pixel.h"

using namespace std;


void Pixel::updatePixel(Pixel* p, int o) {
	double a1 = (double)p->alpha / (double)255 * (double)o / (double)100; //alpha piksela na novom sloju
	double a0 = (double)alpha / (double)255;
	double a = a0 + (1 - a0) * a1;
	int red = r * a0 / a + p->r *(1 - a0)*a1 / a;         
	int green = g * a0 / a + p->g *(1 - a0)*a1 / a;
	int blue = b * a0 / a + p->b *(1 - a0)*a1 / a;
	setR(red);
	setG(green);
	setB(blue);
	setAlpha(a*255);
}
