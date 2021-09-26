#include "rectangle.h"
#include "image.h"
#include "layer.h"

/*void Rectangle::paint(int r, int g, int b, Image *im) {
	for (Layer *lay : im->layers) {
		if (lay->isActive()) {         //ne mora da proverava da li je vec obojeno jer je sve ista boja
			for (int i = x; i < x + h; i++) {
				for (int j = y; j < y + w; j++) {
					lay->getMatrix()[i][j]->setR(r);
					lay->getMatrix()[i][j]->setG(g);
					lay->getMatrix()[i][j]->setB(b);
				}
			}
		}
	}
}*/

void Rectangle::paint(int r, int g, int b, Image *im) {
	for (Layer *lay : im->layers) {
		if (lay->isActive()) {
			vector<vector<Pixel*>> v = lay->getMatrix();
			for_each(v.begin() + x, v.begin() + x + h, [this, &r, &g, &b](vector<Pixel*>& vrow) {
				for_each(vrow.begin() + y, vrow.begin() + y + w, [&r, &g, &b](Pixel *p) {
					p->setR(r);
					p->setG(g);
					p->setB(b);
				});
			});
		}
	}
}


void Rectangle::calc(Operation *operation, map<Pixel*, int>&done, Image *im) {
	for (Layer *lay : im->layers) {
		if (lay->isActive()) {
			vector<vector<Pixel*>> v = lay->getMatrix();
			for_each(v.begin() + x, v.begin() + x + h, [this, operation, &done](vector<Pixel*>& vrow) {
				for_each(vrow.begin() + y, vrow.begin() + y + w, [operation, &done](Pixel *&p) {
					if (done[p] != operation->getNum()) {
						operation->operation(p, operation->getC());
						done[p]++;
					}
				});
			});
		}
	}
}
