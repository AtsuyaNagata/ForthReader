#include "Reader_Lib/XMLReader/Document.h"
#include "FileSystem/File.h"
#include "Reader_Lib/XMLReader/Tag.h"
#include "Reader_Lib/XMLReader/Element.h"

Document::Document() : mRoot(0) {
	mRoot = new Element("<ROOT>");
}

Document::Document(const char* filename) : mRoot(0) {
	//ファイルを全部読み込み
	File file(filename);
	//ダミーの根元エレメント用にタグを用意。名前は<ROOT>
	//<は名前には使えない文字なので、これが特別であることがすぐわかる。
	Tag tag("<ROOT>");
	const char* begin = file.data();
	const char* end = begin + file.size();
	mRoot = new Element(&tag, &begin, end);
}

Document::~Document() {
	delete[] mRoot;
}

void Document::write(const char* filename) const {
	string str;
	//ルート要素は無視してかきこみ
	for (int i = 0; i < mRoot->childNumber(); ++i) {
		mRoot->child(i)->convertToString(&str, 0);
	}
	File::write(filename, str.c_str(), static_cast<int>(str.size()));
}

const Element* Document::root() const {
	return mRoot;
}

Element* Document::root() {
	return mRoot;
}