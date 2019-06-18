#ifndef READER_LIB_XMLREADER_DOCUMENT
#define READER_LIB_XMLREADER_DOCUMENT

class Element;

class Document {
public:
	Document(); //空のものを作る
	Document(const char* filename);
	~Document();
	//書き出し
	void write(const char* filename) const;
	//ルートゲット
	const Element* root() const;
	Element* root(); //書き換え用
private:
	Element* mRoot;
};

#endif