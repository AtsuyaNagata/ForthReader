#include "Reader_Lib/ForthReader/MemoryManager.h"
#include "Reader_Lib/ForthReader/ForthReader.h"
#include "FileSystem/File.h"
#include <iostream>

//0-9のどれかかを調べる関数
bool static isNumber(char c) {
	return 
		(c < '0') ? false :
		(c > '9') ? false : true;
}

bool static isCharacter(char c) {
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' );
}

//デバッグようかなー？
ForthReader::ForthReader() :
	mFile(0)
{
	mStatus = READ_LINE;
}

//本格的なコード読み取りの時に用いる予定
ForthReader::ForthReader(const char* name) :
	mFile(0)
{
	mStatus = READ_FILE;
	mFile = new File(name);
}

ForthReader::~ForthReader() {
	if (mFile) {
		delete mFile;
	}
}

void ForthReader::read()
{
	//ファイルからの読み込み
	if (mStatus == ForthReader::ReadStatus::READ_FILE) {
		readForth(mFile->data());
	}
	//標準入力からの読み込み
	else if (mStatus == ForthReader::ReadStatus::READ_LINE) {
		char line[30];
		//ファイルディスクリプタは標準入力
		fgets(line, sizeof(line), stdin);
		readForth(line);
	}
}

void ForthReader::readForth(const char* source)
{
	//シーケンスモード
	int m = 0;

	const char* p = source;
	while (*p != '\0') {
		char c = *p;
		p++;
		if (c == ' ') {
			//何もしない
		}
		else if(isNumber(c)){
			//数に変換して代入
			int num = c - '0';
			while (isNumber(*p)) {
				num *= 10;
				num += *p - '0';
				p++;
			}
			MemoryManager::instance()->push(num);
		}
	}
}