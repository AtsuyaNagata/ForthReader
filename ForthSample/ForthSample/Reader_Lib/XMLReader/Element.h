#ifndef READER_LIB_XMLREADER_ELEMENT
#define READER_LIB_XMLREADER_ELEMENT

#include <string>
#include <vector>
using namespace std;

class Attribute;
class Tag;

class Element
{
public:
	Element();
	Element(const char* name);	//���O����R���X�g���N�g
	Element(Tag* beginTag, const char** p, const char* end);
	~Element();

	//�A�g���r���[�g�Q�b�g����
	int attributeNumber() const;
	const Attribute* attribute(int i) const;
	Attribute* attribute(int i);
	//�A�g���r���[�g�Z�b�g����
	void setAttributeNumber(int);
	//�q���Q�b�g�n
	int childNumber() const;
	const Element* child(int i) const;
	Element* child(int i);
	//�q���Z�b�g�n
	void setChildNumber(int);
	//���O�Q�b�g
	const string* name() const;
	//���O�ύX
	void setName(const char*);
	//������
	void convertToString(string* out, int indent) const;

private:
	vector< Attribute* > mAttributes;
	vector< Element* > mChildren;
	string mName;
};

#endif