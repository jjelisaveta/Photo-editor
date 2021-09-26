#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "formatterr.h"
#include "image.h"
#include "pixel.h"
#include "layer.h"
#include "bmp_formatter.h"
using namespace std;

void BMPFormatter::readImage(Layer *lay) {
	ifstream file(name, ios::binary | ios::in);
	if (!file.is_open()) {
		cout << "Fajl ne postoji!";
		return;
	}
	char BMPHeader[15], *DIBHeader;
	file.read(BMPHeader, 14);

	int header = *(int*)&BMPHeader[10];   //velicina zaglavlja
	DIBHeader = new char[header - 14];
	file.read(DIBHeader, header - 14);

	int w = *(int*)&DIBHeader[4];
	int h = *(int*)&DIBHeader[8];
	lay->setWidth(w);
	lay->setHeight(h);

	char bgra[5];
	vector < vector < Pixel*>> v(h);
	for (vector<Pixel*>& vrow : v) {
		for (int j = 0; j < w; j++) {
			file.read(bgra, 4);      //jedan piksel
			Pixel *p = new Pixel((unsigned char)bgra[2], (unsigned char)bgra[1], (unsigned char)bgra[0], (unsigned char)bgra[3]);
			vrow.push_back(p);
		}
	}
	reverse(v.begin(), v.end());
	lay->setMatrix(v);
	file.close();	
}

void BMPFormatter::exportImage(Image *im) {
	im->newImage();
	ofstream f(im->name, ios::binary | ios::out);
	
	int offset = 122;
	char zeros[122] = { 0 };
	f.write("BM", 2);   //fiksno
	unsigned int tmpint = offset + im->getHeight()*im->getWidth() * 4;
	f.write((char*)&tmpint, 4);   //velicina u bajtovima
	f.write(zeros, 4);
	tmpint = offset - 14;  
	f.write((char*)&offset, 4);  //velicina zaglavlja
	f.write((char*)&tmpint, 4);
	tmpint = im->getWidth();    
	f.write((char*)&tmpint, 4);   //sirina
	tmpint = im->getHeight();
	f.write((char*)&tmpint, 4);   //visina
	unsigned short tmpshort = 1;
	f.write((char*)&tmpshort, 2);
	tmpshort = 32;
	f.write((char*)&tmpshort, 2);
	f.write(zeros, 4);
	tmpint = im->getHeight()*im->getWidth() * 4;
	f.write((char*)&tmpint, 4);
	tmpint = 2835;
	f.write((char*)&tmpint, 4);
	f.write((char*)&tmpint, 4);
	f.write(zeros, 76);
	char bgra[5];
	vector<vector<Pixel*>> v = im->getPicture();
	
	for_each(v.rbegin(), v.rend(), [&bgra, &f](vector<Pixel*>& vrow) {
		for_each(vrow.begin(), vrow.end(), [&bgra, &f](Pixel *p) {
			bgra[0] = (unsigned char)p->getB();
			bgra[1] = (unsigned char)p->getG();
			bgra[2] = (unsigned char)p->getR();
			bgra[3] = (unsigned char)p->getAlpha();
			f.write(bgra, 4);
		});
	});
	
	f.close();
}

void BMPFormatter::exportLayer(Layer *lay, int n, string proj) {
	string name = proj + "l" + to_string(n) + ".bmp";
	lay->setPath(name);
	ofstream f(name, ios::binary | ios::out);
	int offset = 122;
	char zeros[122] = { 0 };
	f.write("BM", 2);   //fiksno
	unsigned int tmpint = offset + lay->getHeight()*lay->getWidth() * 4;
	f.write((char*)&tmpint, 4);   //velicina u bajtovima
	f.write(zeros, 4);
	tmpint = offset - 14;
	f.write((char*)&offset, 4);  //velicina zaglavlja
	f.write((char*)&tmpint, 4);
	tmpint = lay->getWidth();
	f.write((char*)&tmpint, 4);   //sirina
	tmpint = lay->getHeight();
	f.write((char*)&tmpint, 4);   //visina
	unsigned short tmpshort = 1;
	f.write((char*)&tmpshort, 2);
	tmpshort = 32;
	f.write((char*)&tmpshort, 2);
	f.write(zeros, 4);
	tmpint = lay->getHeight()*lay->getWidth() * 4;
	f.write((char*)&tmpint, 4);
	tmpint = 2835;
	f.write((char*)&tmpint, 4);
	f.write((char*)&tmpint, 4);
	f.write(zeros, 76);
	char bgra[5];
	vector<vector<Pixel*>> v = lay->getMatrix();

	for_each(v.rbegin(), v.rend(), [&bgra, &f](vector<Pixel*>& vrow) {
		for_each(vrow.begin(), vrow.end(), [&bgra, &f](Pixel *p) {
			bgra[0] = (unsigned char)p->getB();
			bgra[1] = (unsigned char)p->getG();
			bgra[2] = (unsigned char)p->getR();
			bgra[3] = (unsigned char)p->getAlpha();
			f.write(bgra, 4);
		});
	});
	f.close();
}