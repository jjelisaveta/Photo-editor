#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <vector>
#include <map>
#include "pixel.h"
#include "layer.h"
#include "rectangle.h"

using namespace std;

class Formatter;
class CompositeOperation;
class Selection;

class Image {
public:
	Image();

	int getWidth() const { return width; }
	void setWidth(int w) { width = w; }
	void setHeight(int h) { height = h; }
	int getHeight() const { return height; }
	vector<Layer*>& getLayers() { return layers; }
	map<string, CompositeOperation*>& getCops() { return cops; }
	void setCops(map<string, CompositeOperation*>& c) { cops = c; }
	map<string, Selection*>& getSelections() { return sel; }
	string getName() const { return name; }
	void setName(string n) { name = n; }
	vector<vector<Pixel*>>& getPicture() { return picture; }
	void setPicture(vector < vector < Pixel*>> v) { picture = v; }

	void addLayer(Layer *lay);
	void addLayer(int h, int w, int o = 0);
	void removeLayer(int num);

	void addSelection(Selection *s);
	void deleteSelection(string name);
	void paintSelection(int r, int g, int b);
	int countActSel() const;

	void updateImage(int num = -1);
	void newImage();
	void exportImage(string newName);
	void expandPicture(int h, int w);
	void addOperation(CompositeOperation *cop);

	~Image();
private:	
	map<string, CompositeOperation*> cops;
	vector<vector<Pixel*>> picture;
	map<string, Selection*> sel;
	int height, width;
	int n;
	vector<Layer*> layers;
	string name;
	void firstLayer(int num);

	friend class BMPFormatter;
	friend class PAMFormatter;
	friend class Rectangle;
};

#endif 