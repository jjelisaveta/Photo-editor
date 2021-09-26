#ifndef FORMATTER_H
#define FORMATTER_H

#include <iostream>
#include <string>
#include "pixel.h"
//#include "image.h"
using namespace std;

class Layer;
class Image;

class Formatter {
public:
	Formatter() {}
	Formatter(string n) : name(n), im(nullptr) {}
	virtual void readImage(Layer *lay) = 0;
	virtual void exportImage(Image* im) = 0;
	~Formatter() { }
protected:
	string name;
	Image *im;
};

#endif