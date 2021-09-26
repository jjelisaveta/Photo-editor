#include <iostream>
#include <vector>
#include "image.h"
#include "pixel.h"
#include "layer.h"
#include "operation.h"
#include "basic_operations.h"
#include "composite_operation.h"
using namespace std;

CompositeOperation::CompositeOperation(string n) {
	name = n;
}

void CompositeOperation::operation(Pixel *p, int c) {
	for (Operation *op : ops) 
		op->operation(p, op->getC());   //urade se sve operacije za piksel
}

void CompositeOperation::addOperation(Operation *op) {
	ops.push_back(op);
}

void CompositeOperation::calc(Image *im) {
	for (Operation *op : ops)      //za svaku operaciju pozove calc
		op->calc(im);
}
