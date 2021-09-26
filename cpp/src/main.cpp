#include <iostream>
#include <cstdlib>
#include <string>
#include "pixel.h"
#include "layer.h"
#include "image.h"
#include "selection.h"
#include "bmp_formatter.h"
#include "pam_formatter.h"
#include "operation.h"
#include "basic_operations.h"
#include "composite_operation.h"
#include "xml_formatter.h"

using namespace std;

/*int main(int argc, const char *argv[]) {

	if (argc == 1) {
		//XMLFormatter *xml = new XMLFormatter();
		//Image *image = xml->readAll("projekat.xml");

		Image *image = new Image();
	    Layer* l = new Layer("png.bmp", 50);
		image->addLayer(l);
		l = new Layer("haha.bmp", 70);
		image->addLayer(l);
		//image->addLayer(new Layer("tica.bmp", 20));

		Rectangle *r = new Rectangle(0, 0, 50, 50);
		vector <Rectangle*> rrr;
		rrr.push_back(r);
		r = new Rectangle(100, 100, 110, 50);
		rrr.push_back(r);
		Selection *selec = new Selection("prva", rrr, true);
		image->addSelection(selec);

		rrr.clear();
		r = new Rectangle(40, 40, 20, 20);
		rrr.push_back(r);
		r = new Rectangle(200, 200, 20, 20);
		rrr.push_back(r);
		selec = new Selection("druga", rrr, true);
		image->addSelection(selec);
		//image->setActiveSel("prva");
		Operation *oper = new Sub(100);
		CompositeOperation *co = new CompositeOperation("najjaca");
		co->addOperation(oper);
		oper = new Median();
		co->addOperation(oper);
		//oper = new Sub(100);
		//co->addOperation(oper);
		image->addOperation(co);
		co->doOper(image);
		//image->paintSelection("prva",100, 0, 200);
	//	image->removeLayer(1);
		XMLFormatter *xml = new XMLFormatter();
		xml->exportImage(image);
		//xml->exportFunction(co);
		image->exportImage("proba.bmp");
		BMPFormatter *bmp = new BMPFormatter();
		bmp->exportImage(image);
		//normalan program
	}
	else if (argc == 3) {
		//poziv metode iz Test fje
	}
	else {
		//pogresan broj parametara - exception
	}

	system("pause");
}*/

int main(int argc, char* argv[]) {
	ofstream output =  ofstream("AAA.txt", ios::out);
	output << argc << endl;
	for (int i = 0; i < argc; i++)
		output << argv[i] << endl;
	if (argc == 1) {
		Image *image = new Image();
		string text;
		CompositeOperation *co = nullptr;
		Operation *op = nullptr;
		Layer *layer;
		bool working = false;
		XMLFormatter *xml = new XMLFormatter();
		int o, tmp, x, y, h, w;
		string path;
		do {
			cout << "Meni:" << endl;
			cout << "   1. dodaj sloj sa slikom" << endl;
			cout << "   2. dodaj prazan sloj" << endl;
			cout << "   3. obrisi sloj" << endl;
			cout << "   4. izmeni vidljivost slojeva" << endl;
			cout << "   5. izmeni aktivnost slojeva" << endl;
			cout << "   6. dodaj selekciju" << endl;
			cout << "   7. izmeni aktivnost selekcija" << endl;
			cout << "   8. oboj selekciju" << endl;
			cout << "   9. obrisi selekciju" << endl;
			cout << "   10. izvrsi operaciju nad slikom" << endl;
			cout << "   11. napravi kompozitnu funkciju" << endl;
			cout << "   12. ucitaj kompozitnu funkciju" << endl;
			cout << "   13. sacuvaj kompozitnu funkciju" << endl;
			cout << "   14. ucitaj projekat" << endl;
			cout << "   15. sacuvaj sliku" << endl;
			cout << "   16. kraj rada" << endl;
			cin >> o;
			switch (o) {
			case 1:
				cout << "Uneti putanju do zeljene slike: ";
				cin >> path;
				cout << "Uneti prozirnost sloja: ";
				cin >> tmp;
				while (tmp < 0 || tmp>100) {
					cout << "Neispravna vrednost prozirnosti. Uneti novu (0-100): ";
					cin >> tmp;
				}
				layer = new Layer(path, tmp);
				if (layer->getMatrix().size()) {
					image->addLayer(layer);
					cout << "Slika je ucitana." << endl;
					working = true;
				}
				else
					cout << "Slika nije ucitana." << endl;
				break;
			case 2:
				cout << "Uneti dimenzije providnog sloja (u pikselima):" << endl << "   -visina: ";
				cin >> x;
				cout << "   -sirina: ";
				cin >> y;
				layer = new Layer(x, y);
				image->addLayer(layer);
				cout << "Providan sloj je dodat." << endl;
				working = true;
				break;
			case 3:
				if (image->getLayers().size() == 0)
					cout << "Nije dodat nijedan sloj!";
				else {
					int i = 1;
					for (auto *lay : image->getLayers())
						cout << i++ << ". sloj (" << lay->getPath() << ") " << endl;
					cout << "Uneti redni broj sloja koji treba obrisati: ";
					cin >> tmp;
					image->removeLayer(tmp);
				}
				break;
			case 4:
				if (image->getLayers().size() == 0)
					cout << "Nije dodat nijedan sloj!";
				else {
					int i = 1;
					for (auto *lay : image->getLayers())
						cout << i++ << ". sloj (" << lay->getPath() << ") " << (lay->isVisible() ? "je" : "nije") << " vidljiv." << endl;
					cout << "Uneti redni broj sloja cija se vidljivost menja: ";
					cin >> tmp;
					image->getLayers()[tmp - 1]->setVisible(!image->getLayers()[tmp - 1]->isVisible());
					cout << "Promenjeno!" << endl;
				}
				break;
			case 5:
				if (image->getLayers().size() == 0)
					cout << "Nije dodat nijedan sloj!";
				else {
					int i = 1;
					for (auto *lay : image->getLayers())
						cout << i++ << ". sloj (" << lay->getPath() << ") " << (lay->isActive() ? "je" : "nije") << " aktivan." << endl;
					cout << "Uneti redni broj sloja cija se aktivnost menja: ";
					cin >> tmp;
					image->getLayers()[tmp - 1]->setActive(!image->getLayers()[tmp - 1]->isActive());
					cout << "Promenjeno!" << endl;
				}
				break;
			case 6:
				if (image->getLayers().size() == 0)
					cout << "Ne moze se dodati selekcija ako slika nije ucitana!" << endl;
				else {
					cout << "Uneti ime selekcije: ";
					cin >> path;
					while (image->getSelections()[path] != nullptr) {
						cout << "Selekcija sa zadatim imenom vec postoji, uneiti novo ime: ";
						cin >> path;
					}
					cout << "Koliko pravouganika dodati u selekciju? ";
					cin >> tmp;
					cout << "Uneti koordinate gornjeg levog ugla, visinu i sirinu pravouganika." << endl;
					Rectangle *rectangle;
					vector<Rectangle*> vecrec;
					for (int i = 0; i < tmp; i++) {
						cout << "Pravougaonik " << i + 1 << ": ";
						cin >> x >> y >> h >> w;
						while (x < 0 || y < 0 || h < 0 || w < 0 || x + w >= image->getWidth() || y + h >= image->getHeight()) {
							cout << "Koordinate neispravne. Uneti nove: ";
							cin >> x >> y >> h >> w;
						}
						rectangle = new Rectangle(x, y, h, w);
						vecrec.push_back(rectangle);
					}
					image->addSelection(new Selection(path, vecrec));
				}
				break;
			case 7:
				if (image->getLayers().size() == 0)
					cout << "Nije dodat nijedan sloj!";
				else {
					int i = 1;
					for (auto sel : image->getSelections())
						cout << i++ << ". selekcija (" << sel.first << ") " << (sel.second->isActive() ? "je" : "nije") << " aktivna." << endl;
					cout << "Uneti ime selekcije cija se aktivnost menja: ";
					cin >> text;
					if (image->getSelections()[text] != nullptr) {
						image->getSelections()[text]->setActive(!image->getSelections()[text]->isActive());
						cout << "Promenjeno!" << endl;
					}
					else
						cout << "Ne postoji selekcija sa zadatim imenom." << endl;
				}
				break;
			case 8:
				cout << "Uneti vrednosti crvene, zelene i plave komponente boje: ";
				cin >> x >> y >> h;
				while (x < 0 || y < 0 || h < 0 || x>255 || y>255 || h>255) {
					cout << "Neispravne vrednosti komponenti. Uneti ponovo (0 - 255): ";
					cin >> x >> y >> h;
				}
				if (image && image->getSelections().size())
					image->paintSelection(x, y, h);
				else
					cout << "Greska: Nemoguce obojiti.";
				break;
			case 9:
				cout << "Uneti ime selekcije koju treba obrisati: " << endl;
				cin >> path;
				image->deleteSelection(path);
				break;
			case 10:
				//operacije
				cout << "Uneti ime operacije koja treba da se izvrsi: ";
				cin.ignore();
				getline(cin, text);
				op = Operation::checkOperation(text);
				if (op)
					op->doOper(image);
				else {
					if (image && image->getCops()[text] != 0)
						image->getCops()[text]->doOper(image);
					else
						cout << "Greska: Neispravna operacija." << endl;
				}
				delete op;
				break;
			case 11:
				cout << "Uneti ime kompozitne operacije: ";
				cin >> text;
				while (image->getCops()[text] != nullptr) {
					cout << "Funkcija sa zadatim imenom vec postoji. Uneti novo: ";
					cin >> text;
				}
				co = new CompositeOperation(text);
				cout << "Uneti broj operacija koje ce sadrzati kompozitna operacija: ";
				cin >> tmp;
				cin.ignore();
				for (int i = 0; i < tmp; i++) {
					cout << i + 1 << ". operacija: ";
					getline(cin, text);
					op = Operation::checkOperation(text);
					if (op == nullptr) {
						if (image && image->getCops()[text] != 0)
							op = image->getCops()[text];
						else {
							cout << "Greska: Neispravna operacija." << endl;
							break;
						}
					}
					co->addOperation(op);
				}
				image->addOperation(co);
				break;
			case 12:
				cout << "Uneti ime kompozitne funkcije koju treba ucitati: ";
				cin >> path;
				co = xml->readFunction(path, image);
				image->addOperation(co);
				break;
			case 13:
				if (co) {
					cout << "Uneti ime funkcije koja se cuva: ";
					cin >> path;
					co->setName(path);
					xml->exportFunction(co);
				}
				else
					cout << "Ne postoji funkcija koja bi mogla biti sacuvana." << endl;
				break;
			case 14:
				if (working) {
					cout << "Postoji slika koja se obradjuje. Da li zelite da ucitate projekat?" << endl << "   1. da" << endl << "   2. ne" << endl;
					cin >> tmp;
					if (tmp == 2) break;
				}
				delete image;
				cout << "Unesite ime projekta koji treba ucitati: ";
				cin >> path;
				image = xml->readAll(path);
				break;
			case 15:
				cout << "Zeljeno ime za cuvanje slike: ";
				cin >> path;
				image->exportImage(path);
				break;

			case 16:
				cout << "Da li zelite da sacuvate projekat?" << endl << "   1. da" << endl << "   2. ne" << endl;
				cin >> tmp;
				if (tmp == 1) {
					XMLFormatter *xml = new XMLFormatter();
					cout << "Unesite ime projekta: ";
					cin >> path;
					xml->exportProject(path, image);
				}
				break;
			}

			system("pause");

			system("cls");
		} while (o != 16);
	}
	else if (argc == 3) {
	for (int i = 0; i < 3; i++)
		cout << argv[i] << endl;
		XMLFormatter* xml = new XMLFormatter();
		Image *image = xml->readAll(argv[0]);
		Operation* op = xml->readFunction(argv[], image);
		op->doOper(image);
		xml->exportProject("result", image);
	}
	else {
		cout << "Neodgovarajuci broj argumenata!" << endl;
	}
}
