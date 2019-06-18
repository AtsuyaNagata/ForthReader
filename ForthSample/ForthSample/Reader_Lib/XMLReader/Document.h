#ifndef READER_LIB_XMLREADER_DOCUMENT
#define READER_LIB_XMLREADER_DOCUMENT

class Element;

class Document
{
public:
	Document(const char* xmlFilename);

private:
	Element* mRoot;

};

#endif