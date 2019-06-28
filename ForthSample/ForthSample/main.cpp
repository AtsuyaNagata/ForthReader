#include<iostream>
#include"FileSystem/File.h"
#include"Reader_Lib/XMLReader/Document.h"
#include"Reader_Lib/XMLReader/Element.h"
#include"Reader_Lib/XMLReader/Attribute.h"
#include"Reader_Lib/ForthReader/MemoryManager.h"
using namespace std;

int main() {
	MemoryManager::create();
	unsigned char p[4] = { 0x00, 0x01, 0x02, 0xff };
	MemoryManager::instance()->push(p);

	printf("%d\n", MemoryManager::instance()->pop());
}

/* XMLリーダの使用

void printElement(Element* e);

int main() {
	Document Sample("Sample.xml");
	//rootの直下
	Element *temporary = Sample.root();
	printElement(temporary);
	return 0;
}
*/
