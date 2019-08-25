#include "Reader_lib/ForthReader/MemoryManager.h"
#include <stdio.h>
#include <stdlib.h>
#define FORTH_MEMORY 2000 

//�ً}�E�o
void emergencyExit() {
	//�t�@�C���̖��O���o�͂�����
	printf("exit file : %s\n", __FILE__);
	exit(1);
}

//�V���O���g���̉�����
MemoryManager* MemoryManager::mMemoryInstance = 0;

MemoryManager::MemoryManager() :
	mMemory(0),
	mStackErr(false)
{
	mMemory = new char[FORTH_MEMORY];
	mEnd = &mMemory[FORTH_MEMORY];		//�I���|�C���^(���̒n�_���Q�Ƃ�����A�E�g)
	pCurrent = mMemory;
}

MemoryManager::~MemoryManager() {
	delete[] mMemory;
	//�ė��p�̉\����O��I�ɂԂ�
	mMemory = 0;
}

void MemoryManager::create() {
	if (!mMemoryInstance) {
		mMemoryInstance = new MemoryManager();
	}
	else {
		//�댯�ȌĂяo���Ȃ̂ŁA�����I�������邱�Ƃł������ȏ����̑���������_��
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