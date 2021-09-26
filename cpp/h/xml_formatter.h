#ifndef XML_FORMATTER
#define XML_FORMATTER

#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include "formatterr.h"
#include "image.h"
#include "pixel.h"
#include "layer.h"
#include "bmp_formatter.h"
#include "selection.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "composite_operation.h"

using namespace std;

class XMLFormatter : public Formatter{
public:
	XMLFormatter() {}
	XMLFormatter(string name) : Formatter(name) { }
	virtual void exportImage(Image *im);
	void exportFunction(CompositeOperation *cop);
	void exportProject(string projName, Image *im);
	virtual void readImage(Layer *lay) {
		//
	}
	Image* readAll(string name);
	CompositeOperation* readFunction(string name, Image *im);
private:

};


#endif
