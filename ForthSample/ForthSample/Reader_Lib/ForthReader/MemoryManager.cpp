#include "Reader_lib/ForthReader/MemoryManager.h"
#include <stdio.h>
#include <stdlib.h>
#define FORTH_MEMORY 2000 

//緊急脱出
void emergencyExit() {
	//ファイルの名前を出力させる
	printf("exit file : %s\n", __FILE__);
	exit(1);
}

//シングルトンの下準備
MemoryManager* MemoryManager::mMemoryInstance = 0;

MemoryManager::MemoryManager() :
	mMemory(0)
{
	//メモリ文のバイト持ってくる
	mMemory = new unsigned char[FORTH_MEMORY];
	//命令領域を総メモリの半分とし、もう半分は記憶領域とする
	mCmdMem = mMemory;
	//記憶領域の先頭ポインタ
	mDataMem = &mMemory[FORTH_MEMORY / 2 - 1];
	//終了ポインタ(この地点を参照したらアウト)
	mEnd = &mMemory[FORTH_MEMORY];

	//スタックポインタを最初の地点にしておく
	mStack = mDataMem;
	//PCを先頭に持っていく
	mPC = mCmdMem;
}

MemoryManager::~MemoryManager() {
	delete[] mMemory;
	//再利用の可能性を徹底的につぶす
	mMemory = 0;
	mCmdMem = 0;
	mDataMem = 0;
	mStack = 0;
	mPC = 0;
}

void MemoryManager::create() {
	if (!mMemoryInstance) {
		mMemoryInstance = new MemoryManager();
	}
	else {
		//危険な呼び出しなので、強制終了させることでおかしな処理の早期発見を狙う
		emergencyExit();
	}
}

MemoryManager* MemoryManager::instance() {
	return mMemoryInstance;
}

void MemoryManager::destroy() {
	delete mMemoryInstance;
	mMemoryInstance = 0;
}

//データはビックエンディアンで扱うことにする 4バイトづつ入れる事に注意
void MemoryManager::push(unsigned char* p) {
	//現在の地点から4バイト分読み込む
	mStack[0] = p[0];
	mStack[1] = p[1];
	mStack[2] = p[2];
	mStack[3] = p[3];
	//スタックポインタを次に備えておく
	mStack += 4;
}

unsigned MemoryManager::pop() {
	//スタックは下げる。ポップされた地点の掃除はしない
	mStack -= 4;
	//unsignedにして返す
	unsigned u = mStack[0] << 24;
	u |= mStack[1] << 16;
	u |= mStack[2] << 8;
	u |= mStack[3];
	return u;
}