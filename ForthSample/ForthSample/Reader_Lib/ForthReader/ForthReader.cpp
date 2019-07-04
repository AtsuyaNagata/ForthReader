#include "Reader_Lib/ForthReader/MemoryManager.h"
#include "Reader_Lib/ForthReader/ForthReader.h"
#include "FileSystem/File.h"
#include <iostream>

//0-9�̂ǂꂩ���𒲂ׂ�֐�
bool static isNumber(char c) {
	return 
		(c < '0') ? false :
		(c > '9') ? false : true;
}

bool static isCharacter(char c) {
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' );
}

//�f�o�b�O�悤���ȁ[�H
ForthReader::ForthReader() :
	mFile(0)
{
	mStatus = READ_LINE;
}

//�{�i�I�ȃR�[�h�ǂݎ��̎��ɗp����\��
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
	//�t�@�C������̓ǂݍ���
	if (mStatus == ForthReader::ReadStatus::READ_FILE) {
		readForth(mFile->data());
	}
	//�W�����͂���̓ǂݍ���
	else if (mStatus == ForthReader::ReadStatus::READ_LINE) {
		char line[30];
		//�t�@�C���f�B�X�N���v�^�͕W������
		fgets(line, sizeof(line), stdin);
		readForth(line);
	}
}

void ForthReader::readForth(const char* source)
{
	//�V�[�P���X���[�h
	int m = 0;

	const char* p = source;
	while (*p != '\0') {
		char c = *p;
		p++;
		if (c == ' ') {
			//�������Ȃ�
		}
		else if(isNumber(c)){
			//���ɕϊ����đ��
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