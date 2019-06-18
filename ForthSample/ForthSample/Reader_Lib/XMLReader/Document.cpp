#include "Reader_Lib/XMLReader/Document.h"
#include "FileSystem/File.h"

Document::Document(const char* filename)
{
	//ファイルをすべて読む
	File file(filename);

	//ダミーの根本エレメント用にタグを用意する。空。
	Tag tag("Dummy");
	//テキスト解釈は行わず、丸投げ
	mRoot = new Element(&tag, file.data());
}