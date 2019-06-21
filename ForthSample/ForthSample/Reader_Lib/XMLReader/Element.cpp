#include "Reader_Lib/XMLReader/Element.h"
#include "Reader_Lib/XMLReader/Tag.h"
#include "Reader_Lib/XMLReader/Attribute.h"

Element::Element(){}

Element::Element(const char* name) : mName(name) {}

//第一引数：どのタグのエレメントを作ろうとしているか	第二：テキストデータをポインタで受け取り	第三：テキストのエンドポインタ
Element::Element(Tag* beginTag, const char** p, const char* e)
{
	//名前を移植：<ROOT>を持ってるやつが基底
	mName = *(beginTag->name());
	//アトリビュートを移植(中身のコピーはせずポインタで貰う)  開放はElementの仕事
	mAttributes = *beginTag->attributes();		//ポインタの値を丸ごとコピー

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
		else {
			++(*p); //<が出るまで無視しまくり
		}
	}
}

//デストラクタ
Element::~Element() {
	for (unsigned i = 0; i < mAttributes.size(); ++i) {
		delete mAttributes[i];
	}
	for (unsigned i = 0; i < mChildren.size(); ++i) {
		delete mChildren[i];
	}
}

//アトリビュートサイズを手に入れる
int Element::attributeNumber() const {
	//size_type型からint型へ	(size_type型は要素を数えるのを専用としたもので符号が無い)
	return static_cast< int >(mAttributes.size());
}

//[i]番目のアトリビュートを獲得
Attribute* Element::attribute(int i) {
	return mAttributes[i];
}

//新しくn個のアトリビュートを作る
void Element::setAttributeNumber(int n) {
	//今ある分は捨てる
	for (unsigned i = 0; i < mAttributes.size(); ++i) {
		delete mAttributes[i];
	}
	mAttributes.resize(n);
	for (int i = 0; i < n; ++i) {
		mAttributes[i] = new Attribute();
	}
}

//子供の数を獲得
int Element::childNumber() const {
	//size_type型からint型へ
	return static_cast<int>(mChildren.size());
}

//[i]番目の子供を獲得
const Element* Element::child(int i) const {
	return mChildren[i];
}

//中身を書き換えそう
Element* Element::child(int i) {
	return mChildren[i];
}

//新たな子供を生成（今までの子供は消えるのであしからず）
void Element::setChildNumber(int n) {
	//今ある分は捨てる
	for (unsigned i = 0; i < mChildren.size(); ++i) {
		delete mChildren[i];
	}
	mChildren.resize(n);
	for (int i = 0; i < n; ++i) {
		mChildren[i] = new Element();
	}
}

//名前を返す。ポインタで
const string* Element::name() const {
	return &mName;
}

//名前の書き換え
void Element::setName(const char* name) {
	mName = name;
}

//書き込みを行う(</Name>のためのインデント)再帰関数
void Element::convertToString(string* out, int indent) const {
	//インデントの数だけタブを書く	(<Name>のためのインデント)
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
	//インデントの数だけタブを書く	(</Name>のためのインデント)
	for (int i = 0; i < indent; ++i) {
		*out += '\t';
	}
	//終了タグ
	*out += "</";
	*out += mName;
	*out += ">\r\n";
}
