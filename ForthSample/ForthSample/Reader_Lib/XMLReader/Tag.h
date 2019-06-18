#ifndef READER_LIB_XMLREADER_TAG
#define READER_LIB_XMLREADER_TAG

#include <iostream>
#include <vector>
using namespace std;
class Element;
class Attribute;

class Tag
{
public:
	enum Type{
		TYPE_BEGIN,
		TYPE_END
	};
	Tag(const char* name); //名前だけのタグを作る
	Tag(const char** readPointer, const char* end);
	~Tag();
	const string* name() const;
	//アトリビュート配列ゲット。重いからポインタ返し。
	const vector< Attribute* >* attributes() const;
	Type type() const;

private:
	vector< Attribute* > mAttributes;	//タグが所有する変数的な物
	string mName;						//タグの名前
	Type mType;							//開始タグか終了タグか
};


/*
Tagクラスが解析を行い、ElementとAttributeを構築する.
Elementは開始タグと終了タグに挟まれており、その間にタグがあったら子要素として吸収する.
つまり、ElementはElementを所有する可能性がある.
今の所、ElementはTagしか理解できない.
*/

#endif