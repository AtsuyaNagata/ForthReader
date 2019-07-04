#include<iostream>
#include"FileSystem/File.h"
#include"Reader_Lib/XMLReader/Document.h"
#include"Reader_Lib/XMLReader/Element.h"
#include"Reader_Lib/XMLReader/Attribute.h"
#include"Reader_Lib/ForthReader/MemoryManager.h"
#include"Reader_Lib/ForthReader/ForthReader.h"
using namespace std;

int main() {
	MemoryManager::create();
	ForthReader reader;

	reader.read();

	printf("%d\n", MemoryManager::instance()->pop());
}

/* XML���[�_�̎g�p

void printElement(Element* e);

int main() {
	Document Sample("Sample.xml");
	//root�̒���
	Element *temporary = Sample.root();
	printElement(temporary);
	return 0;
}
*/
