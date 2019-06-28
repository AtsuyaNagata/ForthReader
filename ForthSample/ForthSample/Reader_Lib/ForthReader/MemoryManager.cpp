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
	mMemory(0)
{
	//���������̃o�C�g�����Ă���
	mMemory = new unsigned char[FORTH_MEMORY];
	//���ߗ̈�𑍃������̔����Ƃ��A���������͋L���̈�Ƃ���
	mCmdMem = mMemory;
	//�L���̈�̐擪�|�C���^
	mDataMem = &mMemory[FORTH_MEMORY / 2 - 1];
	//�I���|�C���^(���̒n�_���Q�Ƃ�����A�E�g)
	mEnd = &mMemory[FORTH_MEMORY];

	//�X�^�b�N�|�C���^���ŏ��̒n�_�ɂ��Ă���
	mStack = mDataMem;
	//PC��擪�Ɏ����Ă���
	mPC = mCmdMem;
}

MemoryManager::~MemoryManager() {
	delete[] mMemory;
	//�ė��p�̉\����O��I�ɂԂ�
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

//�f�[�^�̓r�b�N�G���f�B�A���ň������Ƃɂ��� 4�o�C�g�Â���鎖�ɒ���
void MemoryManager::push(unsigned char* p) {
	//���݂̒n�_����4�o�C�g���ǂݍ���
	mStack[0] = p[0];
	mStack[1] = p[1];
	mStack[2] = p[2];
	mStack[3] = p[3];
	//�X�^�b�N�|�C���^�����ɔ����Ă���
	mStack += 4;
}

unsigned MemoryManager::pop() {
	//�X�^�b�N�͉�����B�|�b�v���ꂽ�n�_�̑|���͂��Ȃ�
	mStack -= 4;
	//unsigned�ɂ��ĕԂ�
	unsigned u = mStack[0] << 24;
	u |= mStack[1] << 16;
	u |= mStack[2] << 8;
	u |= mStack[3];
	return u;
}