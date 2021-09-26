#ifndef BASIC_OPERATIONS_H
#define BASIC_OPERATIONS_H

#include <iostream>
#include "image.h"
#include "pixel.h"
#include "layer.h"
#include "operation.h"

class Add :public Operation {
public:
	Add(int cc = 0) : Operation(cc) {}
	virtual void operation(Pixel *p, int c);
};

class Sub :public Operation {
public:
	Sub(int cc = 0) : Operation(cc) {}
	virtual void operation(Pixel *p, int c);
};

class ReverseSub :public Operation {
public:
	ReverseSub(int cc = 0) : Operation(cc) {}
	virtual void operation(Pixel *p, int c);
};

class Div :public Operation {
public:
	Div(int cc = 0) : Operation(cc) {}
	virtual void operation(Pixel *p, int c);
};

class ReverseDiv :public Operation {
public:
	ReverseDiv(int cc = 0) : Operation(cc) {}
	virtual void operation(Pixel *p, int c);
};

class Mul:public Operation {
public:
	Mul(int cc = 0) : Operation(cc) {}
	virtual void operation(Pixel *p, int c);
};

class Power :public Operation {
public:
	Power(int cc = 0) : Operation(cc) {}
	virtual void operation(Pixel *p, int c);
};

class Log :public Operation {
public:
	Log(int cc = 0) : Operation(cc) {}
	virtual void operation(Pixel *p, int c);
};

class Abs :public Operation {
public:
	Abs(int cc = 0) : Operation(cc) {}
	virtual void operation(Pixel *p, int c);
};

class Min :public Operation {
public:
	Min(int cc = 0) : Operation(cc) {}
	virtual void operation(Pixel *p, int c);
};

class Max :public Operation {
public:
	Max(int cc = 0) : Operation(cc) {}
	virtual void operation(Pixel *p, int c);
};

class Inversion :public Operation {
public:
	Inversion(int cc = 0) : Operation(cc) {}
	virtual void operation(Pixel *p, int c);
};

class Grayscale :public Operation {
public:
	Grayscale(int cc = 0) : Operation(cc) {}
	virtual void operation(Pixel *p, int c);
};

class BlackNWhite :public Operation {
public:
	BlackNWhite(int cc = 0) : Operation(cc) {}
	virtual void operation(Pixel *p, int c);
};

class Median :public Operation {
public:
	Median(int cc = 0) : Operation(cc) {}
	virtual void operation(Pixel *p, int c);
	virtual void calc(Image *im);
	void calcMedian(int, int, int, int, map<Pixel*, int>&, int&);
	void setRGB(int *r, int*g, int*b, int i, int j, int &n, vector<vector<Pixel*>>& v);
private:
	int findMedian(int *arr, int n);
};

#endif