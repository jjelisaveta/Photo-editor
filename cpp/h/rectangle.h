#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <map>
#include "pixel.h"
#include "operation.h"
using namespace std;

class Image;
class Operation;

class Rectangle {
public:
	Rectangle(int xx, int yy, int hh, int ww) : x(xx), y(yy), h(hh), w(ww) {}

	int getX() const { return x; }
	int getY() const { return y; }
	int getH() const { return h; }
	int getW() const { return w; }


	void paint(int r, int g, int, Image *im);
	void calc(Operation *operation, map<Pixel*, int>& done, Image *im);
private:
	int x, y, h, w;
};

#endif