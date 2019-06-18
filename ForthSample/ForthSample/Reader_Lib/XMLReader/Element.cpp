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


int Element::childNumber() const {
	return static_cast<int>(mChildren.size());
}

const Element* Element::child(int i) const {
	return mChildren[i];
}

Element* Element::child(int i) {
	return mChildren[i];
}

void Element::setChildNumber(int n) {
	//今ある分は捨てる
	for (unsigned i = 0; i < mChildren.size(); ++i) {
		delete[] mChildren[i];
	}
	mChildren.resize(n);
	for (int i = 0; i < n; ++i) {
		mChildren[i] = new Element();
	}
}

const string* Element::name() const {
	return &mName;
}

void Element::setName(const char* name) {
	mName = name;
}

//再帰関数
void Element::convertToString(string* out, int indent) const {
	//インデントの数だけタブを書く
	for (int i = 0; i < indent; ++i) {
		*out += '\t';
	}
	//タグ開始とエレメント名
	*out += '<';
	*out += mName;
	//アトリビュート書き込み
	for (unsigned i = 0; i < mAttributes.size(); ++i) {
		if (mAttributes[i]) {
			*out += ' '; //スペース
			*out += *(mAttributes[i]->name());	//開始タグ内のアトリビュート集
			*out += "=\"";						//「="」を記入
			*out += *(mAttributes[i]->value());	//値を入れる
			*out += '"';
		}
	}
	*out += ">\r\n"; //開始タグ終わり
	//子に流す
	for (unsigned i = 0; i < mChildren.size(); ++i) {
		//子の要素があれば
		if (mChildren[i]) {
			//子のエレメントを展開しておく。この時、インデントを下げることでかっこつける
			mChildren[i]->convertToString(out, indent + 1);
		}
	}
	//インデントの数だけタブを書く
	for (int i = 0; i < indent; ++i) {
		*out += '\t';
	}
	//終了タグ
	*out += "</";
	*out += mName;
	*out += ">\r\n";
}
