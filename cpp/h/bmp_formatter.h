#ifndef BMP_FORMATTER_H
#define BMP_FORMATTER_H

#include <iostream>
#include <fstream>
#include "formatterr.h"
#include "image.h"
#include "pixel.h"
#include "layer.h"
using namespace std;

class BMPFormatter : public Formatter {
public:
	BMPFormatter() {}
	BMPFormatter(string name) : Formatter(name) { }
	virtual void readImage(Layer *lay);
	virtual void exportImage(Image* im);
	void exportLayer(Layer *lay, int n, string proj);
};

#endif