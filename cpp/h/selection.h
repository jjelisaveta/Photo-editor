#ifndef SELECTION_H
#define SELECTION_H

#include <vector>
#include <map>
#include <iostream>
#include "rectangle.h"
#include "operation.h"
#include "basic_operations.h"
#include "composite_operation.h"

using namespace std;


class Selection {
public:
	Selection(string n, vector<Rectangle*> r, bool a = false) : name(n), rectangles(r), active(a) {};

	vector<Rectangle*> getRectangles() { return rectangles; }
	void setActive(bool a) { active = a; }
	bool isActive() { return active; }
	string getName() { return name; }
	void setName(string n) { name = n; }

	void paint(int r, int g, int b, Image *im);
	void calc(Operation* operation, Image *im, map<Pixel*, int>& done);
	~Selection();
private:
	vector<Rectangle*> rectangles;
	bool active;
	string name;
};

#endif