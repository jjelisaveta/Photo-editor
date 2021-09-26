#include "layer.h"


Layer::Layer(int h, int w, int o) {
	height = h;
	width = w;
	opacity = o;
	active = true;
	visible = true;
	vector<vector<Pixel*>> vm(h);
	for_each(vm.begin(), vm.end(), [&w](vector<Pixel*>& vrow) {
		for (int j = 0; j < w; j++) 
			vrow.push_back(new Pixel(0, 0, 0, 0));
	});
	mat = vm;
}

Layer::Layer(string p, int o) {
	opacity = o;
	path = p;
	Formatter *f = checkFormat();
	if (f) {
		f->readImage(this);
		active = visible = true;
	}
	else
		cout << "Greska!";
}

Formatter* Layer::checkFormat() {
	regex rx("([a-zA-Z0-9]*.).(...)");
	smatch res;
	string extension;
	if (regex_match(path, res, rx)) {
		extension = res.str(2);
	}
	else {
		cout << "Neodgovarajuci format." << endl;
		return nullptr;
	}
	Formatter *f = nullptr;
	if (extension == "pam")
		f = new PAMFormatter(path);
	else if (extension == "bmp")
		f = new BMPFormatter(path);
	else if (extension == "xml")
		f = new XMLFormatter(path);
	else
		cout << "Neodgovarajuci format." << endl;
	return f;
}

void Layer::expandLayer(int h, int w) {
	for (int i = height; i < h; i++) {    //ako treba dodati na dno
		vector<Pixel*> v;
		mat.push_back(v);   
		for (int j = 0; j < width; j++) {    //dodaje width broj prozirnih (traka na dnu), pocetna sirina
			Pixel *p = new Pixel(0, 0, 0, 0);
			mat[i].push_back(p);
		}
	}
	if (w > width) {    //samo ako ima potrebe da se prosiruje
		for (int i = 0; i < h; i++) {
			for (int j = width; j < w; j++) {
				Pixel *p = new Pixel(0, 0, 0, 0);
				mat[i].push_back(p);
			}
		}
	}
	height = h;
	width = w;
}

Layer::~Layer() {
	for_each(mat.begin(), mat.end(), [](vector<Pixel*>& row) {
		for_each(row.begin(), row.end(), [](Pixel* &p) {
			delete p;
		});
	});
	mat.erase(mat.begin(), mat.end());
}