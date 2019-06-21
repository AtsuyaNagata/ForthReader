#include<iostream>
#include"FileSystem/File.h"
#include"Reader_Lib/XMLReader/Document.h"
#include"Reader_Lib/XMLReader/Element.h"
#include"Reader_Lib/XMLReader/Attribute.h"
using namespace std;

void printElement(Element* e);

int main() {
	Document Sample("Sample.xml");
	//root‚Ì’¼‰º
	Element *temporary = Sample.root();
	printElement(temporary);
	return 0;
}

void printElement(Element *e) {
	for (int i = 0; i < e->childNumber(); ++i) {
		printf("%s:   ", (*e->child(i)->name()).c_str());
		for (int j = 0; j < e->child(i)->attributeNumber(); ++j) {
			printf("Attribute %d: name=%s, value=%s    ", j+1, (*e->child(i)->attribute(j)->name()).c_str(), (*e->child(i)->attribute(j)->value()).c_str());
		}
		printf("\n");
		if (e->child(i)->childNumber() > 0) {
			printElement(e->child(i));
		}
	}
}
