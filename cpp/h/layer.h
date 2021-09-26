#ifndef LAYER_H
#define LAYER_H

#include <iostream>
#include <vector>
#include <regex>
#include "pixel.h"
#include "rectangle.h"
#include "formatterr.h"
#include "pam_formatter.h"
#include "bmp_formatter.h"
#include "xml_formatter.h"

using namespace std;

class Layer {
private:
	int width, height, opacity;
	vector<vector<Pixel*>> mat;
	bool active, visible;
	string path;
public:
	Layer(string p, int o = 100);
	Layer(int h, int w, int o = 0);

	bool isActive() const { return active; }
	void setActive(bool a) { active = a; }
	bool isVisible() const { return visible; }
	void setVisible(bool v) { visible = v; }
	int getWidth() const { return width; }
	void setWidth(int w) { width = w; }
	int getHeight() const { return height; }
	void setHeight(int h) { height = h; }
	int getOpacity() const { return opacity; }
	void setOpacity(int o) { opacity = o; }
	string getPath() const { return path; }
	void setPath(string p) { path = p; }
	vector<vector<Pixel*>>& getMatrix() { return mat; }
	void setMatrix(vector<vector<Pixel*>>& m) { mat = m; }

	Formatter* checkFormat();
	void expandLayer(int h, int w);

	~Layer();

	friend class Image;
	friend class Formatter;
	friend class BMPFormatter;
	friend class PAMFormatter;
	friend class XMLFormatter;
	friend void Rectangle::paint(int r, int g, int b, Image *im);
};

#endif