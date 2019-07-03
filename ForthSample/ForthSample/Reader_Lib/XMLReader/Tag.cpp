#include "Reader_Lib/XMLReader/Tag.h"
#include "FileSystem/File.h"
#include "Reader_Lib/XMLReader/Attribute.h"

//a-z,A-Z,0-9,_のどれかかを調べる関数
//わかりやすいが遅い。高速化の方法を検討してみよう。
bool static isNormalChar(char c) {
	if (c >= '0' && c <= '9') {
		return true;
	}
	if (c >= 'a' && c <= 'z') {
		return true;
	}
	if (c >= 'A' && c <= 'Z') {
		return true;
	}
	if (c == '_') {
		return true;
	}
	return false;
}

//状態遷移表。最低限なので、エラーには異常なほど寛容な作りになっている。
/*
<状態番号リスト>
0:<の上。初期状態
1:エレメント名文字列
2:エレメント名の後の空白。アトリビュートの間の空白。
3:アトリビュート名
4:=の上
5:アトリビュート値(""の中身)
E:終了

<状態遷移リスト>
記号:
c = [a-zA-Z0-9_] つまり普通の文字
s = 他の条件に引っかからない残りの文字

0,/,0 終了タグフラグ立てる
0,s,1 エレメント名文字列最初の一文字
1,c,1 エレメント名文字列に追加
1,>,E
1,s,2 エレメント名確定
2,>,E
2,c,3 アトリビュート名最初の一文字
2,s,2 読み飛ばす
3,=,4 =発見。アトリビュート名確定
3,s,3 アトリビュート名に追加
4,",5 ダブルクォテイション発見。アトリビュート値に移行
4,s,4 読み飛ばす
5,",2 アトリビュート値確定。アトリビュートをリストに追加。
5,s,5 アトリビュート値文字列に追加
*/


//<ROOT>を生成する際に用いる
Tag::Tag(const char* name) :
	mName(name),
	mType(TYPE_BEGIN) {
}

Tag::Tag(const char** p, const char* e) :
	mType(TYPE_BEGIN)
{
	//アトリビュートの名前と値の一時的な格納場所
	string name;
	string value;

	int m = 0;			//シーケンスモード。使用可数が多いので一文字
	bool end = false;	//ループを抜けるための変数
	while (*p < e) {
		char c = **p;	//一文字抽出
		++(*p);			//一つ進める
		switch (m) {

			case 0:								//初期状態
				switch (c) {
					case '/':	mType = TYPE_END; break;	//終了タグ
					default: mName += c; m = 1; break;		//エレメント名突入
				}
				break;
			
			case 1:								//エレメント名の読み取り
				if (c == '>') {
					end = true;								//終了準備
				}
				else if (isNormalChar(c)) {		//許可のある文字だったら
					mName += c;					//文字をぶち込む
				}
				else {
					m = 2;									//エレメント名の処理終了
				}
				break;

			case 2:								//エレメント名の後の空白
				if (c == '>') {
					end = true;
				}
				else if (isNormalChar(c)) {
					name += c;								//アトリビュート名追加
					m = 3;
				}
				else {
					;										//何もしない
				}
				break;

			case 3:								//アトリビュート名
				switch (c) {
					case '=':	m = 4;	break;				//「=」が来たら即抜け
					default:	name += c; break;			//アトリビュート名に文字を追加
				}
				break;

			case 4:											//「=」後の処理
				switch (c) {
					case '"':	m = 5;	break;
					default:	break;						//何もしない
				}
				break;

			case 5:								//アトリビュート値
				switch (c) {
					case '"':
						m = 2;
						//アトリビュート追加(詳細なし)
						mAttributes.push_back(new Attribute( name.c_str(), value.c_str() ) );
						//名前と値を初期化
						name.clear();
						value.clear();
						break;
						
					default: value += c; break;
				}
				break;
		}
		if (end) {
			break;
		}
	}
}

Tag::~Tag() {
	//作ったタグは必ずElementの材料になっているはずで、
	//mAttributesの中身は渡してあるのでdeleteはしない
}

const string* Tag::name() const {
	return &mName;
}

Tag::Type Tag::type() const {
	return mType;
}

//重いのでポインタ返し
const vector< Attribute* >* Tag::attributes() const {
	return &mAttributes;
}
