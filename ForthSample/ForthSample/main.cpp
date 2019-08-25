#include<iostream>
#include"FileSystem/File.h"
#include"Reader_Lib/ForthReader/MemoryManager.h"
#include"Reader_Lib/ForthReader/ForthReader.h"
using namespace std;

int main() {
	MemoryManager::create();
	ForthReader forth = ForthReader();

	forth.read();

	//printf("%d\n", MemoryManager::instance()->pop());
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
