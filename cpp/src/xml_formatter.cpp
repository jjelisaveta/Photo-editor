#include "xml_formatter.h"
#include <regex>

using namespace std;

class CompositeOperation;

void XMLFormatter::exportImage(Image *im) {
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);
	BMPFormatter *bmp = new BMPFormatter();
	string projName = im->getName();
	im->setName(projName + ".bmp");
	bmp->exportImage(im);
	TiXmlElement * root = new TiXmlElement("Project");
	doc.LinkEndChild(root);
	TiXmlElement *lays = new TiXmlElement("Layers");

	/*dodaje slojeve*/
	root->LinkEndChild(lays);
	int cnt = 0;
	for (auto layer : im->getLayers()) {
		bmp->exportLayer(layer, cnt++, projName);
		TiXmlElement *oneLayer = new TiXmlElement("Layer");

		TiXmlElement *path = new TiXmlElement("path");
		TiXmlText * text = new TiXmlText(layer->getPath().c_str());
		path->LinkEndChild(text);

		TiXmlElement *opac = new TiXmlElement("opacity");
		text = new TiXmlText(to_string(layer->getOpacity()).c_str());
		opac->LinkEndChild(text);

		TiXmlElement *vis = new TiXmlElement("visible");
		text = new TiXmlText(to_string((int)layer->isVisible()).c_str());
		vis->LinkEndChild(text);

		TiXmlElement *act = new TiXmlElement("active");
		text = new TiXmlText(to_string((int)layer->isActive()).c_str());
		act->LinkEndChild(text);

		oneLayer->LinkEndChild(path);
		oneLayer->LinkEndChild(opac);
		oneLayer->LinkEndChild(vis);
		oneLayer->LinkEndChild(act);
		lays->LinkEndChild(oneLayer);
	}

	/*dodaje selekcije*/
	TiXmlElement *sels = new TiXmlElement("Selections");
	for (auto oneSel : im->getSelections()) {
		TiXmlElement *oneSelection = new TiXmlElement("Selection");

		TiXmlElement *name = new TiXmlElement("name");
		TiXmlText * text = new TiXmlText(oneSel.first.c_str());
		name->LinkEndChild(text);
		TiXmlElement *allRectangles = new TiXmlElement("Rectangles");
		for (auto recs : oneSel.second->getRectangles()) {
			TiXmlElement *oneRec = new TiXmlElement("Rectangle");

			TiXmlElement *x = new TiXmlElement("x");
			text = new TiXmlText(to_string((int)recs->getX()).c_str());
			x->LinkEndChild(text);

			TiXmlElement *y = new TiXmlElement("y");
			text = new TiXmlText(to_string((int)recs->getY()).c_str());
			y->LinkEndChild(text);

			TiXmlElement *h = new TiXmlElement("h");
			text = new TiXmlText(to_string((int)recs->getH()).c_str());
			h->LinkEndChild(text);

			TiXmlElement *w = new TiXmlElement("w");
			text = new TiXmlText(to_string((int)recs->getW()).c_str());
			w->LinkEndChild(text);

			oneRec->LinkEndChild(x);
			oneRec->LinkEndChild(y);
			oneRec->LinkEndChild(h);
			oneRec->LinkEndChild(w);
			allRectangles->LinkEndChild(oneRec);

		}

		oneSelection->LinkEndChild(name);
		oneSelection->LinkEndChild(allRectangles);
		sels->LinkEndChild(oneSelection);
	}

	TiXmlElement *ops = new TiXmlElement("Operations");
	for (auto allCops : im->getCops()) {
		TiXmlElement *oneOper = new TiXmlElement("CompositeOperation");

		TiXmlElement *name = new TiXmlElement("name");
		TiXmlText * text = new TiXmlText(allCops.first.c_str());
		name->LinkEndChild(text);
		oneOper->LinkEndChild(name);
		ops->LinkEndChild(oneOper);

	}


	root->LinkEndChild(sels);
	root->LinkEndChild(ops);
	projName.append(".xml");
	doc.SaveFile(projName.c_str());
}

void XMLFormatter::exportFunction(CompositeOperation *cop) {
	TiXmlDocument doc; 
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement * root = new TiXmlElement("CompositeOperation");
	doc.LinkEndChild(root);

	for (auto op : cop->getOps()) {
		cout << typeid(*op).name() << '|' << endl;
		if (strcmp(typeid(*op).name(), "class CompositeOperation")==0) {
			TiXmlElement *oneOperation = new TiXmlElement("CompositeOperation");
			CompositeOperation *cop = dynamic_cast<CompositeOperation*> (op);
			TiXmlText *text = new TiXmlText(cop->getName().c_str());
			oneOperation->LinkEndChild(text);
			root->LinkEndChild(oneOperation);
		}
		else {
			TiXmlElement *oneOperation = new TiXmlElement("Operation");

			TiXmlElement *type = new TiXmlElement("type");
			TiXmlText *text = new TiXmlText(typeid(*op).name());
			type->LinkEndChild(text);

			TiXmlElement *constant = new TiXmlElement("c");
			text = new TiXmlText(to_string(op->getC()).c_str());
			constant->LinkEndChild(text);

			oneOperation->LinkEndChild(type);
			oneOperation->LinkEndChild(constant);

			root->LinkEndChild(oneOperation);
		}
	}
	string name = cop->getName();
	name.append(".fun");
	doc.SaveFile(name.c_str());
}

void XMLFormatter::exportProject(string projName, Image *im) {

	for (auto cop : im->getCops()) {
		exportFunction(cop.second);
	}
	im->setName(projName);
	exportImage(im);

}

Image* XMLFormatter::readAll(string name) {
	ifstream file(name, ios::in);
	Image *image = new Image();
	if (!file.is_open()) {
		cout << "Projekat sa zadatim imenom ne postoji!" << endl;
		return nullptr;
	}
	else {
		cout << "Projekat otvoren!" << endl;
	}
	smatch res;
	string text;
	getline(file, text);  //zaglavlje
	getline(file, text);
	regex rxHeader ("<([a-zA-Z0-9]*)>");
	if (regex_match(text, res, rxHeader)) {
		if (res.str(1) != "Project") {
			cout << "Neodgovarajuci format!" << endl;
			return nullptr;
		}
	}
	rxHeader = "[^<]*<([^>]*)>";
	getline(file, text);
	cout << text << endl;
	if (regex_match(text, res, rxHeader)) {
		if (res.str(1) == "Layers /") {
			cout << "Ucitan projekat nema nijedan sloj!" << endl;
		}
		else {
			regex rxH("[^<]*<Layer>");
			regex rxRes("[^<]*<path>([a-zA-Z0-9]*).(...)</path>[^<]*<opacity>([0-9]*)</opacity>[^<]*<visible>(.)</visible>[^<]*<active>(.)</active>[^<]*</Layer>");
			getline(file, text);
			string tmp;
			while (true) {
				if (regex_match(text, res, rxH)) {
					text = "";
					for (int i = 0; i < 5; i++) {
						getline(file, tmp);
						text.append(tmp);
					}
					if (regex_match(text, res, rxRes)) {
						Layer *layer = new Layer(res.str(1).append(".").append(res.str(2)), stoi(res.str(3)));
						if (layer->getMatrix().size() == 0)
							return nullptr;
						layer->setVisible(stoi(res.str(4)));
						layer->setActive(stoi(res.str(5)));
						//cout << stoi(res.str(3)) << endl;
						image->addLayer(layer);
					}
					getline(file, text);
				}
				else
					break;   //procitao nesto sto nije pocetak novog Layer-a  (</Layers>)
			}
		}
	}

	//citanje selekcija
	getline(file, text);
	cout << text << endl;
	string selName;
	if (regex_match(text, res, rxHeader)) {
		if (res.str(1) == "Selections /") {
			cout << "Ucitan projekat nema nijednu selekciju!" << endl;
		}
		else {
			regex rxH("[^<]*<Selection>");
			regex rxRes("[^<]*<name>([a-zA-Z0-9]*)</name>");
			getline(file, text);
			string tmp;
			while (regex_match(text, res, rxH)) {
				getline(file, text);
				if (regex_match(text, res, rxRes)) {
					selName = res.str(1);
					cout << selName << endl;
				}
				getline(file, text);
				if (regex_match(text, res, rxHeader)) {
					if (res.str(1) == "Rectangles /")   //ne moze da se desi
						cout << "Selekcija \"" << name << "\" nema pravougainika!" << endl;
					else {
						vector<Rectangle*> vecrec;
						getline(file, text);
						rxH = "[^<]*<Rectangle>";
						rxRes = "[^<]*<x>([0-9]*)</x>[^<]*<y>([0-9]*)</y>[^<]*<h>([0-9]*)</h>[^<]*<w>([0-9]*)</w>[^<]*</Rectangle>";
						while (true) {
							if (regex_match(text, res, rxH)) {
								text = "";
								for (int i = 0; i < 5; i++) {
									getline(file, tmp);
									text.append(tmp);
								}
								if (regex_match(text, res, rxRes)) {
									Rectangle *rec = new Rectangle(stoi(res.str(1)), stoi(res.str(2)), stoi(res.str(3)), stoi(res.str(4)));
									vecrec.push_back(rec);
								}
								getline(file, text);
							}
							else
								break;   //procitao nesto sto nije pocetak novog Layer-a
						}
						Selection *sel = new Selection(selName, vecrec);
						image->addSelection(sel);
						rxH = "[^<]*<Selection>";
						getline(file, text);
					}
				}
			}
			getline(file, text);
		}
	}

	getline(file, text);
	cout << text << endl;
	string opName;
	if (regex_match(text, res, rxHeader)) {
		if (res.str(1) == "Operations /") {
			cout << "Ucitan projekat nema nijednu kompozitnu operaciju!" << endl;
		}
		else {
			string path;
			regex rxH("[^<]*<CompositeOperation>");
			getline(file, text);
			map<string, CompositeOperation*> mco;
			CompositeOperation *co = nullptr;
			while (regex_match(text, res, rxH)) {
				getline(file, text);
				regex rxO("[^<]*<name>([^<]*)</name>");
				if (regex_match(text, res, rxO)) {
					path = res.str(1);
				}
				else {
					cout << "Greska: Ime funkcije ne postoji." << endl;
					return nullptr;
				}
				co = readFunction(path, im);
				mco[path] = co;
				getline(file, text);
				getline(file, text);
			}
			image->setCops(mco);
		}
	}
	return image;
}


CompositeOperation* XMLFormatter::readFunction(string name, Image *im) {
	string newName = name + ".fun";
	TiXmlDocument doc(newName.c_str());
	if (!doc.LoadFile()) {
		cout << "Ne postoji sacuvana funkcija sa zadatim imenom." << endl;
		return nullptr;
	}
	CompositeOperation *co = new CompositeOperation(name);
	Operation *o = nullptr;
	string text;
	int c;
	TiXmlNode *root = doc.FirstChild(), *child;  //zaglavlje
	TiXmlElement *elem;
	root = root->NextSibling();//<CompositeOperation
	root = root->FirstChild();
	cout << root->Value();
	while (root) {
		if (strcmp(root->Value(),"Operation")==0) {
			child = root->FirstChild();
			text = child->ToElement()->GetText();
			child = child->NextSibling();
			c = atoi(child->ToElement()->GetText());
			o = Operation::checkOperation(text, c);
		}
		else if (strcmp(root->Value(),"CompositeOperation")==0) {
			text = root->ToElement()->GetText();
			//cout << text << endl;
			o = readFunction(text, im);
		}
		co->addOperation(o);
		root = root->NextSibling();
	}
	return co;
	
}