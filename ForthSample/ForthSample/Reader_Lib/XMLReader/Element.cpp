#include "Reader_Lib/XMLReader/Element.h"
#include "Reader_Lib/XMLReader/Tag.h"
#include "Reader_Lib/XMLReader/Attribute.h"

Element::Element(){}

Element::Element(const char* name) : mName(name) {}

//第一引数：どのタグのエレメントを作ろうとしているか	第二：テキストデータをポインタで受け取り	第三：テキストのエンドポインタ
Element::Element(Tag* beginTag, const char** p, const char* e)
{
	//名前を移植
	mName = *(beginTag->name());
	//アトリビュートを移植(コピーはせずポインタで貰う)
	mAttributes = *beginTag->attributes();

	//タグ探し
	while (*p < e) {
		if (**p == '<') {	//子になりそうなやつを発見！
			++(*p);			//「<」の次へ移動
			Tag tag(p, e);	//pからeまでののタグ解析を丸投げ
			Tag::Type type = tag.type();
			if (type == Tag::TYPE_BEGIN) {	//要素が </Element> だったら子ではない
				mChildren.push_back(new Element(&tag, p, e));	//子に追加
			}
			else if (type == Tag::TYPE_END) {
				++(*p);		//「<」が来るまで無視し続ける！
			}
		}
	}
}

Element::~Element() {
	for (unsigned i = 0; i < mAttributes.size(); ++i) {
		delete(mAttributes[i]);
		mAttributes[i] = 0;
	}
	for (unsigned i = 0; i < mChildren.size(); ++i) {
		delete[] (mChildren[i]);
		mAttributes[i] = 0;
	}
}

int Element::attributeNumber() const {
	return static_cast< int >(mAttributes.size());
}

Attribute* Element::attribute(int i) {
	return mAttributes[i];
}

void Element::setAttributeNumber(int n) {
	//今ある分は捨てる
	for (unsigned i = 0; i < mAttributes.size(); ++i) {
		delete[] mAttributes[i];
	}
	mAttributes.resize(n);
	for (int i = 0; i < n; ++i) {
		mAttributes[i] = new Attribute();
	}
}
