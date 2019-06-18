#ifndef READER_LIB_XMLREADER_DOCUMENT
#define READER_LIB_XMLREADER_DOCUMENT

class Element;

class Document {
public:
	Document(); //��̂��̂����
	Document(const char* filename);
	~Document();
	//�����o��
	void write(const char* filename) const;
	//���[�g�Q�b�g
	const Element* root() const;
	Element* root(); //���������p
private:
	Element* mRoot;
};

#endif