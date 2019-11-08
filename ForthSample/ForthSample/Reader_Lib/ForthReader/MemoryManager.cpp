#include "Reader_lib/ForthReader/MemoryManager.h"
#include <stdio.h>
#include <stdlib.h>

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
	Ifnum(0),
	nowLoopStack(0),
	err(0b00000000)
{
	mMemory = new char[FORTH_MEMORY];
	mEnd = &mMemory[FORTH_MEMORY];		//�I���|�C���^(���̒n�_���Q�Ƃ�����A�E�g)
	pCurrent = mMemory + 8;				//�J��2�|�b�v�ɂ��A�N�Z�X�ᔽ�̉\�������邩��+8
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