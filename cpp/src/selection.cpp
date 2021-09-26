#include "selection.h"

void Selection::paint(int r, int g, int b, Image *im) {   
	if (isActive()) 
		for (Rectangle *rec : rectangles) rec->paint(r, g, b, im);
	
}

void Selection::calc(Operation* operation, Image *im, map<Pixel*, int>& done) {
	for (Rectangle *rec : rectangles) rec->calc(operation, done, im);
}

Selection::~Selection() {
	rectangles.clear();
}
