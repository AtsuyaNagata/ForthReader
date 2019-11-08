#include "Reader_lib/ForthReader/MemoryManager.h"
#include <stdio.h>
#include <stdlib.h>

//緊急脱出
void emergencyExit() {
	//ファイルの名前を出力させる
	printf("exit file : %s\n", __FILE__);
	exit(1);
}

//シングルトンの下準備
MemoryManager* MemoryManager::mMemoryInstance = 0;

MemoryManager::MemoryManager() :
	mMemory(0),
	Ifnum(0),
	nowLoopStack(0),
	err(0b00000000)
{
	mMemory = new char[FORTH_MEMORY];
	mEnd = &mMemory[FORTH_MEMORY];		//終了ポインタ(この地点を参照したらアウト)
	pCurrent = mMemory + 8;				//開幕2ポップによるアクセス違反の可能性があるから+8
}

MemoryManager::~MemoryManager() {
	delete[] mMemory;
	//再利用の可能性を徹底的につぶす
	mMemory = 0;
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

void MemoryManager::push(int param) {
	if (pCurrent + 4 > mEnd) {
		err |= 0b00000001;
		return;
	}
	char* p = reinterpret_cast<char*>(&param);
	pCurrent[0] = p[0];
	pCurrent[1] = p[1];
	pCurrent[2] = p[2];
	pCurrent[3] = p[3];
	pCurrent += 4;
	err &= 0b11111110;
}

void MemoryManager::push(float param) {
	if (pCurrent + 4 > mEnd) {
		err |= 0b00000001;
		return;
	}
	char* p = reinterpret_cast<char*>(&param);
	pCurrent[0] = p[0];
	pCurrent[1] = p[1];
	pCurrent[2] = p[2];
	pCurrent[3] = p[3];
	pCurrent += 4;
	err &= 0b11111110;
}

float MemoryManager::pop(){
	if (pCurrent - 4 < mMemory) {
		err |= 0b00000001;
		return 0;
	}
	pCurrent -= 4;
	err &= 0b11111110;
	return *reinterpret_cast<float*>(pCurrent);
}