#include "Reader_Lib/ForthReader/MemoryManager.h"
#include "Reader_Lib/ForthReader/ForthReader.h"
#include "FileSystem/File.h"
#include <iostream>

#define ORDERSIZE 10

//0-9のどれかかを調べるマン
bool static isNumber(char c) {
	return (c >= '0' && c <= '9');
}
//文字列かどうかを調べるマン
bool static isCharacter(char c) {
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' );
}
//演算子を見つけるマン
bool static isOperator(char c) {
	return ((c == '+') || (c == '-') || (c == '*') || (c == '/'));
}

//コマンドの処理群
void const voidCmd(int, char**);
void const pulusCmd(int, char**);
void const minusCmd(int, char**);
void const multipleCmd(int, char**);
void const divideCmd(int, char**);
void const printCmd(int, char**);
void const forCmd(int, char**);

//デバッグようかなー？

ForthReader::ForthReader() :
	mFile(0),
	forthCmd{
		{"+", pulusCmd},
		{"-", minusCmd},
		{"*", multipleCmd},
		{"/", divideCmd},
		{".", printCmd},
		{"for", forCmd}
	}
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
		//可変になるような調正が必要
		char line[30];
		//ファイルディスクリプタは標準入力
		fgets(line, sizeof(line), stdin);
		readForth(line);
	}
}

void ForthReader::readForth(const char* source)
{



	/*
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
		else if (isOperator(c)) {
			char cmd = ((c == '+') * 1 + ((c == '-') * 2) + ((c == '*') * 3) + (c == '/') * 4);
			MemoryManager::instance()->insertOrder(cmd);

		}
		else if (isCharacter(c)) {
			int sum = c;
			const char* text = p;
			while (isCharacter(*text)) {
				sum += *text;
			}
			specificOrder(sum, text);
		}
		
	}
	*/
}