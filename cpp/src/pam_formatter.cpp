#include <iostream>
#include <fstream>
#include "formatterr.h"
#include "image.h"
#include "pixel.h"
#include "layer.h"
#include "pam_formatter.h"
using namespace std;

void PAMFormatter::readImage(Layer *lay) {
	ifstream file(name, ios::binary | ios::in);
	if (!file.is_open()) {
		cout << "Fajl ne postoji!";  
		return;
	}
	char tmp[255];
	int h, w, depth, maxval;
	char type[15];

	file >> tmp >> tmp >> w >> tmp >> h >> tmp >> depth >> tmp >> maxval >> tmp >> type >> tmp;

	lay->setHeight(h);
	lay->setWidth(w);
	char rgba[5];
	file.read(tmp, 1);   //0x0a
	vector < vector < Pixel*>> v(h);
	for (vector<Pixel*>& vrow : v) {
		for (int j = 0; j < w; j++) {
			file.read(rgba, 4);
			Pixel *p = new Pixel((unsigned char)rgba[0], (unsigned char)rgba[1], (unsigned char)rgba[2], (unsigned char)rgba[3]);
			vrow.push_back(p);
		}
	}
	lay->setMatrix(v);
	file.close();
}

void PAMFormatter::exportImage(Image* im){
	im->newImage();
	ofstream file(im->getName(), ios::binary | ios::out);
	if (!file) 
		cout << "Greska" << endl;
	file << "P7" << (unsigned char)0x0a;
	file << "WIDTH " << im->getWidth() << (unsigned char)0x0a;
	file << "HEIGHT " << im->getHeight() << (unsigned char)0x0a;
	file << "DEPTH " << 4 << (unsigned char)0x0a;
	file << "MAXVAL " << 255 << (unsigned char)0x0a;
	file << "TUPLTYPE RGB_ALPHA" << (unsigned char)0x0a;
	file << "ENDHDR" << (unsigned char)0x0a;

	char rgba[5];
	vector<vector<Pixel*>> v = im->getPicture();
	for_each(v.begin(), v.end(), [&rgba, &file](vector<Pixel*>& vrow) {
		for_each(vrow.begin(), vrow.end(), [&rgba, &file](Pixel *p) {
			rgba[0] = (unsigned char)p->getR();
			rgba[1] = (unsigned char)p->getG();
			rgba[2] = (unsigned char)p->getB();
			rgba[3] = (unsigned char)p->getAlpha();
			file.write(rgba, 4);
		});
	});
	file.close();
}
