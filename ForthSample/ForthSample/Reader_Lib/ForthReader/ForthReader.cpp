#include "Reader_Lib/ForthReader/MemoryManager.h"
#include "Reader_Lib/ForthReader/ForthReader.h"
#include "FileSystem/File.h"
#include <iostream>

#define MAX_CMDNUMBER 6
#define MAX_CMDLENGTH 2

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
//特殊文字見つけるマン
bool static isSpCharacter(char c) {
	return ((c == ' ') || (c == '\0') || (c == '\n') || (c == '\t'));
}

//文字列を数字に変えるマン
double CharacterToNumber(const char* name) {
	double sum = 0;
	//インデックスを指定するための変数
	int i, count;

	//ステージ1 : 小数点以下を考えずに処理
	for (count = 0; isNumber(name[count]); ++count);

	for (i = 0; i < count; ++i) {
		sum *= 10;
		sum += name[i] - '0';
	}

	//もし、区切られ文字が「'.'」の時、少数以下の可能性あり
	if (name[count] == '.') {
		//ステージ2 : 少数点の値の獲得
		i = count + 1;
		for (count = i; isNumber(name[count]); ++count);

		//係数kを用いて少数を表現
		double k = 0.1;
		for (; i < count; ++i) {
			sum += (name[i] - '0') * k;
			k *= 0.1;
		}
	}
	return sum;
}

//コマンドの処理群
int const voidCmd();
int const pulusCmd();
int const minusCmd();
int const multipleCmd();
int const divideCmd();
int const printCmd();
int const ifCmd();
int const thenCmd();
int const DoCmd();
int const LoopCmd();

//デバッグようかなー？
ForthReader::ForthReader() :
	mFile(0),
	forthCmd{
		{"+", pulusCmd},
		{"-", minusCmd},
		{"*", multipleCmd},
		{"/", divideCmd},
		{".", printCmd},
		{"if", ifCmd},
		{"then", thenCmd},
		{"Do", DoCmd}
	}
{
	mStatus = READ_LINE;
}

//本格的なコード読み取りの時に用いる予定
ForthReader::ForthReader(const char* name) :
	mFile(0),
	forthCmd{
		{"+", pulusCmd},
		{"-", minusCmd},
		{"*", multipleCmd},
		{"/", divideCmd},
		{".", printCmd},
		{"if", ifCmd},
		{"Do", DoCmd}
	}
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
		//TODO:可変になるような調正が必要
		char line[30];
		//ファイルディスクリプタは標準入力
		printf("input : ");
		fgets(line, sizeof(line), stdin);
		readForth(line);
	}
}

void ForthReader::readForth(const char* source)
{
	MemoryManager* mem = MemoryManager::instance();

	const char* p = source;
	char cmd[MAX_CMDLENGTH + 1];
	int i = 0, j = 0;
	while (p[i] != '\0') {
		if (isNumber(p[i])) {
			mem->push(static_cast<float>(CharacterToNumber(&p[i])));
			for (; !isSpCharacter(p[i]); ++i)
				if (!isNumber(p[i]) && p[i] != '.')		//不正な文字を検出する
					mem->pop();							//不正な数字を吐き出す
			for (; !isSpCharacter(p[i]); ++i);			//iの値を進めておく
		}
		else {
			for (int j = 0; j < MAX_CMDLENGTH + 1; ++j) { cmd[j] = '\0'; }	//cmdをクリーンする
			for (int j = 0; !isSpCharacter(p[i]); ++i, ++j) {	//cmdは0から、pはiから始める
				//jがコマンドの最大文字数を超えたら特殊文字が来るまでiを進めてbreakさせる
				if (j >= MAX_CMDLENGTH) {
					for (; !isSpCharacter(p[i]); ++i);
					break;
				}
				cmd[j] = p[i];
			}
			
			
			if (executCmd(cmd)) {	//もしエラーが見つかっていたら強制終了
				return;				//エラー内容の出力についてはexectCmdが責任を持つ
			}
		}
		i = i + 1;
	}
}

//コマンド確定、実行するマン
int ForthReader::executCmd(const char* cmdName) {
	int errType = 0;
	//コマンドの数や文字数はそこまで大きくないので2重ループもおｋ
	for (int i = 0; i < MAX_CMDNUMBER; ++i) {		//定数を何とかしたいけど、思いつかないから妥協
		//printf("cmdName = %s  forthCmd[%d].name = %s\n", cmdName, i, forthCmd[i].name); //チェック用
		if (!strcmp(forthCmd[i].name, cmdName)) {	//一致した時実行
			if (errType = forthCmd[i].func() != 0) {
				switch (errType) {
				case 1:
					printf("Stack Error\n");
					return 1;
					break;
				}
			}
			else {
				return 0;
			}
		}
	}
	printf("%s is not defined\n", cmdName);
	return 1;
}


//何もしない事が保証されたコマンド関数
int const voidCmd() {
	return 0;
}

//足し算関数  1はスタックエラー
int const pulusCmd() {
	float ans = MemoryManager::instance()->pop() + MemoryManager::instance()->pop();
	if (MemoryManager::instance()->isStackErr()) {
		return 1;		//スタックエラー
	}
	//2つポップして一つプッシュだからエラーの確認はなしでよい
	MemoryManager::instance()->push(ans);
	return 0;
}

//引き算関数
int const minusCmd() {
	float ans = -MemoryManager::instance()->pop() + MemoryManager::instance()->pop();
	if (MemoryManager::instance()->isStackErr()) {
		return 1;		//スタックエラー
	}
	//2つポップして一つプッシュだからエラーの確認はなしでよい
	MemoryManager::instance()->push(ans);
	return 0;
}

//かけ算関数
int const multipleCmd() {
	float ans = MemoryManager::instance()->pop() * MemoryManager::instance()->pop();
	if (MemoryManager::instance()->isStackErr()) {
		return 1;		//スタックエラー
	}
	//2つポップして一つプッシュだからエラーの確認はなしでよい
	MemoryManager::instance()->push(ans);
	return 0;
}

//割り算関数
int const divideCmd() {
	float divideBuff = MemoryManager::instance()->pop();
	float ans = MemoryManager::instance()->pop() / divideBuff;
	if (MemoryManager::instance()->isStackErr()) {
		return 1;		//スタックエラー
	}
	//2つポップして一つプッシュだからエラーの確認はなしでよい
	MemoryManager::instance()->push(ans);
	return 0;
}

//出力関数(スタックの一番上をポップして出力する)
int const printCmd() {
	float printBuff = MemoryManager::instance()->pop();
	if (MemoryManager::instance()->isStackErr()) {
		return 1;		//スタックエラー
	}
	//改行までが責任とする
	printf("output : ");
	printf("%f\n", printBuff);
	return 0;
}

int const ifCmd() {
	int ifBuff = MemoryManager::instance()->pop();
	if (ifBuff) {
		//コードスキップ機能もしくはコードロック
	}
	else {

	}

	return 0;
}

int const thenCmd() {
	return 0;
}

int const DoCmd() {
	return 0;
}

int const LoopCmd() {
	return 0;
}