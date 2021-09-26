#ifndef OPERATION_H
#define OPERATION_H

#include <iostream>


class Image;
class Pixel;

class Operation {
public:
	Operation(int cc = 0);
	virtual void calc(Image *im);
	virtual void round(Image *im);
	virtual void operation(Pixel *p, int c) = 0;
	virtual void doOper(Image *im);
	static Operation* checkOperation(string name, int cc = 0);

	virtual int getC() const { return c; }
	virtual int getNum() const { return num; }
	virtual void setNum(int n) { num = n; }
protected:
	static const int MAX = 255;
	static const int MIN = 0; 
private:
	int c;
	int num;
};

#endif