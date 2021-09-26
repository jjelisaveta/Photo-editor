#ifndef PAM_FORMATTER_H
#define PAM_FORMATTER_H

#include <iostream>
#include <fstream>
#include "formatterr.h"
#include "image.h"
#include "pixel.h"
#include "layer.h"
using namespace std;



class PAMFormatter : public Formatter {
public:
	PAMFormatter(){}
	PAMFormatter(string name) : Formatter(name) {}
	virtual void readImage(Layer *lay);
	virtual void exportImage(Image* im);
};

#endif