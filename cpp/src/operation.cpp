#include <iostream>
#include <vector>
#include <regex>
#include "image.h"
#include "pixel.h"
#include "selection.h"
#include "layer.h"
#include "operation.h"


Operation::Operation(int cc) {
	c = cc;
	num = 1;
}

void Operation::calc(Image *im) {
	//primeni operaciju na selekcije, odnosno na celu sliku ako nema nijedne aktivne selekcije 
	vector < vector < Pixel*>> v;
	int h = im->getHeight();
	int w = im->getWidth();

	if (im->countActSel() != 0) {
		map<Pixel*, int> done;
		for (auto as: im->getSelections()) 
			as.second->calc(this, im, done);
	}
	else {
		for (Layer *layer : im->getLayers()) {
			v = layer->getMatrix();
			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					operation(v[i][j], c);
				}
			}
		}
	}
}

void Operation::round(Image *im) {
	vector < vector < Pixel*>> v;
	int h = im->getHeight();
	int w = im->getWidth();

	for (Layer *layer : im->getLayers()) {
		v = layer->getMatrix();
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				if (v[i][j]->getR() < Operation::MIN) v[i][j]->setR(Operation::MIN);
				else if (v[i][j]->getR() > Operation::MAX) v[i][j]->setR(Operation::MAX);
				if (v[i][j]->getG() < Operation::MIN) v[i][j]->setG(Operation::MIN);
				else if (v[i][j]->getG() > Operation::MAX) v[i][j]->setG(Operation::MAX);
				if (v[i][j]->getB() < Operation::MIN) v[i][j]->setB(Operation::MIN);
				else if (v[i][j]->getB() > Operation::MAX) v[i][j]->setB(Operation::MAX);
			}
		}
	}
}

void Operation::doOper(Image *im) {
	this->calc(im);
	round(im);
}

Operation* Operation::checkOperation(string name, int cc) {
	Operation *o = nullptr;
	string lname = "";
	regex rx1("class ([a-zA-Z]*)");
	
	regex rx2("([a-zA-Z]*)( [0-9]*)?");
	smatch res;
	if (regex_match(name, res, rx1)) {
		name = res.str(1);
	}
	else if (regex_match(name, res, rx2)) {
		if (res.str(2) != "") {
			string num = res.str(2);
			num.erase(remove_if(num.begin(), num.end(), isspace), num.end());
			cc = stoi(num);
		}
		//cout << "Broj " << cc;
		name = res.str(1);
		//cout << name;
	}
	else
		return nullptr;
	for (auto ch : name) {
		lname += tolower(ch);
	}
	
	if (lname == "add")
		o = new Add(cc);
	else if (lname == "sub")
		o = new Sub(cc);
	else if (lname == "reversesub")
		o = new ReverseSub(cc);
	else if (lname == "mul")
		o = new Mul(cc);
	else if (lname == "div")
		o = new Div(cc);
	else if (lname == "reversediv")
		o = new ReverseDiv(cc);
	else if (lname == "power")
		o = new Power(cc);
	else if (lname == "log")
		o = new Log(cc);
	else if (lname == "abs")
		o = new Abs(cc);
	else if (lname == "min")
		o = new Min(cc);
	else if (lname == "max")
		o = new Max(cc);
	else if (lname == "inversion")
		o = new Inversion(cc);
	else if (lname == "grayscale")
		o = new Grayscale(cc);
	else if (lname == "blacknwhite")
		o = new BlackNWhite(cc);
	else if (lname == "median")
		o = new Median(cc);
	else return nullptr;
	return o;
}