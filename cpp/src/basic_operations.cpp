#include <iostream>
#include <cmath>
#include <algorithm>
#include "image.h"
#include "pixel.h"
#include "layer.h"
#include "operation.h"
#include "selection.h"
#include "basic_operations.h"

using namespace std;

void Add::operation(Pixel *p, int c) {
	p->setR(p->getR() + c);
	p->setG(p->getG() + c);
	p->setB(p->getB() + c);}

void Sub::operation(Pixel *p, int c) {
	p->setR(p->getR() - c);
	p->setG(p->getG() - c);
	p->setB(p->getB() - c);
}

void ReverseSub::operation(Pixel *p, int c) {
	p->setR(c - p->getR());
	p->setG(c - p->getG());
	p->setB(c - p->getB());
}

void Div::operation(Pixel *p, int c) {
	p->setR(p->getR() / c);
	p->setG(p->getG() / c);
	p->setB(p->getB() / c);
}

void ReverseDiv::operation(Pixel *p, int c) {
	p->setR(c / p->getR());
	p->setG(c / p->getG());
	p->setB(c / p->getB());
}

void Mul::operation(Pixel *p, int c) {
	p->setR(p->getR() * c);
	p->setG(p->getG() * c);
	p->setB(p->getB() * c);
}

void Power::operation(Pixel *p, int c) {
	p->setR(pow(p->getR(), c));
	p->setG(pow(p->getG(), c));
	p->setB(pow(p->getB(), c));
}

void Log::operation(Pixel *p, int c) {
	p->setR(log(p->getR()));
	p->setG(log(p->getG()));
	p->setB(log(p->getB()));
}

void Abs::operation(Pixel *p, int c) {
	p->setR(abs(p->getR()));
	p->setG(abs(p->getG()));
	p->setB(abs(p->getB()));
}

void Min::operation(Pixel *p, int c) {
	if (p->getR() > c) p->setR(c);
	if (p->getG() > c) p->setG(c);
	if (p->getB() > c) p->setB(c);
}

void Max::operation(Pixel *p, int c) {
	if (p->getR() < c) p->setR(c);
	if (p->getG() < c) p->setG(c);
	if (p->getB() < c) p->setB(c);
}

void Inversion::operation(Pixel *p, int c) {
	p->setR(Operation::MAX - p->getR());
	p->setG(Operation::MAX - p->getG());
	p->setB(Operation::MAX - p->getB());
}

void Grayscale::operation(Pixel *p, int c) {
	int avg = (p->getR() + p->getG() + p->getB()) / 3;
	p->setR(avg);
	p->setG(avg);
	p->setB(avg);
}

void BlackNWhite::operation(Pixel *p, int c) {
	int avg = (p->getR() + p->getG() + p->getB()) / 3;
	if (avg > 127) {
		p->setR(Operation::MIN);
		p->setG(Operation::MIN);
		p->setB(Operation::MIN);
	}
	else {
		p->setR(Operation::MAX);
		p->setG(Operation::MAX);
		p->setB(Operation::MAX);
	}
}

void Median::operation(Pixel *p, int c) {
	p->setR(c);   //ovoj f-ji se prosledjuje vrednost koja treba da se postavi jer zavisi od pozicije
	p->setG(c);
	p->setB(c);
}

void Median::calc(Image *im) {
	int h = im->getHeight();
	int w = im->getWidth();
	vector<vector<Pixel*>> v;
	
	if (im->countActSel() != 0) {
		map<Pixel*, int> done;
		for (Layer *layer : im->getLayers()) {
			vector<vector<Pixel*>> copy(layer->getHeight());
			for (int i = 0; i < layer->getHeight(); i++) {
				vector<Pixel*> row(layer->getWidth());
				copy[i] = row;
			}
			v = layer->getMatrix();
			for (auto as : im->getSelections()) {
				if (as.second->isActive()) {
					for (auto rec : as.second->getRectangles()) {
						for (int i =0; i < h; i++) {
							for (int j = 0; j < w; j++) {
								if (i >= rec->getY() && i < rec->getY() + rec->getH() && j >= rec->getX() && j < rec->getX() + rec->getW()) {
									if (done[layer->getMatrix()[i][j]] ==0 || done[layer->getMatrix()[i][j]] == 2) {
										int n = 0;
										int r[8], g[8], b[8];
										if (i > rec->getY())
											setRGB(r, g, b, i - 1, j, n, v);
										if (j > rec->getX())
											setRGB(r, g, b, i, j - 1, n, v);
										if (i < h - 1)
											setRGB(r, g, b, i + 1, j, n, v);
										if (j < w - 1)
											setRGB(r, g, b, i, j + 1, n, v);
										if (i > rec->getY() && j > rec->getX())
											setRGB(r, g, b, i - 1, j - 1, n, v);
										if (i > rec->getY() && j < w - 1)
											setRGB(r, g, b, i - 1, j + 1, n, v);
										if (i < h - 1 && j > rec->getX())
											setRGB(r, g, b, i + 1, j - 1, n, v);
										if (i < h - 1 && j < w - 1)
											setRGB(r, g, b, i + 1, j + 1, n, v);
										int red = findMedian(r, n - 1);
										int green = findMedian(g, n - 1);
										int blue = findMedian(b, n - 1);
										Pixel *p = new Pixel(red, green, blue, v[i][j]->getAlpha());
										if (done[layer->getMatrix()[i][j]] == 0) {
											copy[i][j] = p;
											done[layer->getMatrix()[i][j]]++;
										}
										else {
											copy[i][j] = p;
											done[layer->getMatrix()[i][j]] = 1;
										}
									}
								}
								else {
									if (done[layer->getMatrix()[i][j]] == 0) {
										Pixel *p = new Pixel(layer->getMatrix()[i][j]->getR(), layer->getMatrix()[i][j]->getG(), layer->getMatrix()[i][j]->getB(), layer->getMatrix()[i][j]->getAlpha());
										done[layer->getMatrix()[i][j]]=2;
										copy[i][j] = p;
									}
								}
							}
						}
						cout << layer->getHeight() << endl;
						layer->getMatrix().clear();
						layer->setMatrix(copy);
					}
				}
			}
			
		}
	}
	else {
		for (Layer *layer : im->getLayers()) {
			vector<vector<Pixel*>> copy;
			v = layer->getMatrix();
			for (int i = 0; i < h; i++) {
				vector<Pixel*> vcopy;
				copy.push_back(vcopy);
				for (int j = 0; j < w; j++) {
					int n = 0;
					int r[8], g[8], b[8];
					if (i > 0) 
						setRGB(r, g, b, i - 1, j, n, v);
					if (j > 0) 
						setRGB(r, g, b, i, j - 1, n, v);
					if (i < h - 1) 
						setRGB(r, g, b, i + 1, j, n, v);
					if (j < w - 1) 
						setRGB(r, g, b, i , j + 1, n, v);
					if (i > 0 && j > 0) 
						setRGB(r, g, b, i - 1, j - 1, n, v);
					if (i > 0 && j < w - 1) 
						setRGB(r, g, b, i - 1, j + 1, n, v);
					if (i < h - 1 && j > 0) 
						setRGB(r, g, b, i + 1, j - 1, n, v);
					if (i < h - 1 && j < w - 1) 
						setRGB(r, g, b, i + 1, j + 1, n,v);
					int red = findMedian(r, n - 1);
					int green = findMedian(g, n - 1);
					int blue = findMedian(b, n - 1);
					Pixel *p = new Pixel(red, green, blue, v[i][j]->getAlpha());
					copy[i].push_back(p);
				}
			}
			layer->getMatrix().clear();
			layer->setMatrix(copy);
		}
	}
}

int Median::findMedian(int *arr, int n) {
	sort(arr, arr+n);
	return arr[n / 2];
}

void Median::setRGB(int *r, int*g, int*b, int i, int j, int &n, vector<vector<Pixel*>>& v) {
	r[n] = v[i][j]->getR();
	g[n] = v[i][j]->getG();
	b[n++] = v[i][j]->getB();
}
