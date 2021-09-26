#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <regex>
#include <string>
#include "image.h"
#include "formatterr.h"
#include "bmp_formatter.h"
#include "pam_formatter.h"
#include "xml_formatter.h"
#include "composite_operation.h"
#include "selection.h"

using namespace std;

Image::Image() {
	width = height = -1;
}

void Image::addLayer(Layer *lay) {
	int ww = max(width, lay->getWidth());
	int hh = max(height, lay->getHeight());
	layers.push_back(lay);
	if (ww != lay->getWidth() || hh != lay->getHeight() || width != ww || height!=hh) {
		for (Layer *l : layers) l->expandLayer(hh, ww);    //prodji kroz sve slojeve i prosiri manje slojeve
		expandPicture(hh, ww);
	}
	height = hh;
	width = ww;
	updateImage();
}

void Image::addLayer(int h, int w, int o) {  
	Layer *lay = new Layer(h, w, o);
	Image::addLayer(lay);
}

int Image::countActSel() const {
	int cnt = 0;
	for (auto s : sel)
		if (s.second->isActive())
			cnt++;
	return cnt;
}

void Image::addSelection(Selection *s) {
	sel[s->getName()] = s;
}

void Image::paintSelection(int r, int g, int b) {
	if (countActSel()) {
		for_each(sel.begin(), sel.end(), [this, r, g, b](pair<string, Selection*> p) {
			p.second->paint(r,g,b,this);
		});
		cout << "Obojeno!" << endl;
	}
	else
		cout << "Ne postoji aktivna selekcija." << endl;
}

void Image::deleteSelection(string name) {
	if (sel[name] != nullptr) {
		delete sel[name];
		sel[name] = nullptr;
		cout << "Obrisano!" << endl;
	}
	else
		cout << "Trazena selekcija ne postoji!" << endl;
}


void Image::updateImage(int num) {
	if (num == -1 && layers.size()) num = layers.size() - 1;
	else num = 0;
	if (layers.size() > 1) {
		//dodaje se poslednji sloj na postojecu sliku
		Layer* last = layers[num];    //poslednji
		if (last->visible) {
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					picture[i][j]->updatePixel(last->getMatrix()[i][j], last->getOpacity());
				}	
			}
		}
	}
	else {
		firstLayer(num); 
	}
}

void Image::expandPicture(int h, int w) {
	for (int i = height; i < h; i++) {    //vrti se samo ako je h>height
		vector<Pixel*> v;
		picture.push_back(v);    //dodaje prazan vekor
		for (int j = 0; j < width; j++) {    //dodaje width broj prozirnih (traka na dnu), pocetna sirina
			Pixel *p = new Pixel(0, 0, 0, 0);
			picture[i].push_back(p);
			
		}
	}
	if (w > width) {    //ulazi ovde samo ako ima potrebe da se prosiruje
		for (int i = 0; i < h; i++) {
			for (int j = width; j < w; j++) {
				Pixel *p = new Pixel(0, 0, 0, 0);
				picture[i].push_back(p);
			}
		}
	}
}

void Image::firstLayer(int num) {
	Layer *first = layers[num];
	vector<vector<Pixel*>> v(height);
	picture = v;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int a = first->getOpacity()*first->getMatrix()[i][j]->getAlpha() / 100;
			Pixel *p = new Pixel(first->getMatrix()[i][j]->getR(), first->getMatrix()[i][j]->getG(), first->getMatrix()[i][j]->getB(), a);
			picture[i].push_back(p);
		}
	}
}

void Image::newImage() {
	picture.clear();   //obrisi sve
	bool firAdded = false;
	for (int i = 0; i < layers.size(); i++) {
		if (firAdded) {
			Layer* last = layers[i];     //dodaje se poslednji sloj na postojecu sliku
			if (last->visible) {
				for (int i = 0; i < height; i++) 
					for (int j = 0; j < width; j++) 
						picture[i][j]->updatePixel(last->getMatrix()[i][j], last->getOpacity());
			}
		}
		else {
			if (layers[i]->isVisible()) {
				firstLayer(i);
				firAdded = true;
			}
		}
	}
	
}


void Image::exportImage(string newName) {
	regex rx("([a-zA-Z0-9]*.).(...)");
	smatch res;
	string extension;
	name = newName;
	if (regex_match(newName, res, rx)) {
		extension = res.str(2);
	}
	Formatter *f = nullptr;
	if (extension == "pam")
		f = new PAMFormatter(newName);
	else if (extension == "bmp")
		f = new BMPFormatter(newName);
	else if (extension == "xml")
		f = new XMLFormatter(newName);
	f->exportImage(this);
}

void Image::removeLayer(int num) {
	if (num<0 || num>layers.size())
		cout << "Greska";
	layers.erase(layers.begin() + num - 1);
	newImage();
}

void Image::addOperation(CompositeOperation *cop) {
	if (cops[cop->getName()] == nullptr) {
		cops[cop->getName()] = cop;
	}
	else {
		cout << "Funkcija sa zadatim imenom vec postoji! Uneti novo ime: ";
		string name;
		cin >> name;
		if (cops[name] == nullptr) {
			cop->setName(name);
			cops[name] = cop;
		}
		else
			cout << "Greska: Uneto neispravno ime!" << endl;
	}
}

Image::~Image() {
	for (auto s : sel)
		delete s.second;
	for (auto c : cops)
		delete c.second;
	for_each(layers.begin(), layers.end(), [](Layer* l) {
		delete l;
	});
	for_each(picture.begin(), picture.end(), [](vector<Pixel*>& row) {
		for_each(row.begin(), row.end(), [](Pixel* &p) {
			delete p;
		});
	});
}
