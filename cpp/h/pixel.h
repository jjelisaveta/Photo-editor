#ifndef PIXEL_H
#define PIXEL_H

#include <iostream>

using namespace std;

class Pixel {
public:
	Pixel(int rr = 0, int gg = 0, int bb = 0, int a = 100) : r(rr), g(gg), b(bb), alpha(a) {};

	int getR() const { return r; }
	void setR(int rr) { r = rr; }
	int getG() const { return g; }
	void setG(int gg) { g = gg; }
	int getB() const { return b; }
	void setB(int bb) { b = bb; }
	int getAlpha() const { return alpha; }
	void setAlpha(int a) { alpha = a; }

	void updatePixel(Pixel* p, int o);
private:
	int r, g, b, alpha;
};

#endif