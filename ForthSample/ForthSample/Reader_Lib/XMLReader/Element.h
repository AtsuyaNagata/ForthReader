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
	Element(const char* name);	//名前からコンストラクト
	Element(Tag* beginTag, const char** p, const char* end);
	~Element();

	//アトリビュートゲットだぜ
	int attributeNumber() const;
	const Attribute* attribute(int i) const;
	Attribute* attribute(int i);
	//アトリビュートセットだぜ
	void setAttributeNumber(int);
	//子供ゲット系
	int childNumber() const;
	const Element* child(int i) const;
	Element* child(int i);
	//子供セット系
	void setChildNumber(int);
	//名前ゲット
	const string* name() const;
	//名前変更
	void setName(const char*);
	//文字列化
	void convertToString(string* out, int indent) const;

private:
	vector< Attribute* > mAttributes;
	vector< Element* > mChildren;
	string mName;
};

#endif