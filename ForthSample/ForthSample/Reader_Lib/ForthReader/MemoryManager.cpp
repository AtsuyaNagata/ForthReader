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
	mMemory(0),
	mStackErr(false)
{
	mMemory = new char[FORTH_MEMORY];
	mEnd = &mMemory[FORTH_MEMORY];		//終了ポインタ(この地点を参照したらアウト)
	pCurrent = mMemory;
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
		mStackErr = true;
		return;
	}
	char* p = reinterpret_cast<char*>(&param);
	pCurrent[0] = p[0];
	pCurrent[1] = p[1];
	pCurrent[2] = p[2];
	pCurrent[3] = p[3];
	pCurrent += 4;
	mStackErr = false;
}

void MemoryManager::push(float param) {
	if (pCurrent + 4 > mEnd) {
		mStackErr = true;
		return;
	}
	char* p = reinterpret_cast<char*>(&param);
	pCurrent[0] = p[0];
	pCurrent[1] = p[1];
	pCurrent[2] = p[2];
	pCurrent[3] = p[3];
	pCurrent += 4;
	mStackErr = false;
}

float MemoryManager::pop(){
	if (pCurrent - 4 < mMemory) {
		mStackErr = true;
		return 0;
	}
	pCurrent -= 4;
	mStackErr = false;
	return *reinterpret_cast<float*>(pCurrent);
}