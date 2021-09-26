#ifndef COMPOSITE_OPERATION_H
#define COMPOSITE_OPERATION_H

#include <iostream>
#include <vector>
#include "image.h"
#include "pixel.h"
#include "layer.h"
#include "operation.h"
using namespace std;

class CompositeOperation : public Operation {
public:
	CompositeOperation(string n);

	string getName() const { return name; }
	void setName(string n) { name = n; }

	virtual void operation(Pixel *p, int c);
	void addOperation(Operation *op);
	virtual void calc(Image *im);
	vector<Operation*>& getOps() { return ops; }
private:
	vector<Operation*> ops;
	string name;
};

#endif