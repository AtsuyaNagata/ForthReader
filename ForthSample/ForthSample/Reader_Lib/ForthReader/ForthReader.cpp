#include "Reader_Lib/ForthReader/MemoryManager.h"
#include "Reader_Lib/ForthReader/ForthReader.h"
#include "FileSystem/File.h"
#include <iostream>

#define ORDERSIZE 10

//0-9�̂ǂꂩ���𒲂ׂ�}��
bool static isNumber(char c) {
	return (c >= '0' && c <= '9');
}
//�����񂩂ǂ����𒲂ׂ�}��
bool static isCharacter(char c) {
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' );
}
//���Z�q��������}��
bool static isOperator(char c) {
	return ((c == '+') || (c == '-') || (c == '*') || (c == '/'));
}

//�R�}���h�̏����Q
void const voidCmd(int, char**);
void const pulusCmd(int, char**);
void const minusCmd(int, char**);
void const multipleCmd(int, char**);
void const divideCmd(int, char**);
void const printCmd(int, char**);
void const forCmd(int, char**);

//�f�o�b�O�悤���ȁ[�H

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
		//�ςɂȂ�悤�Ȓ������K�v
		char line[30];
		//�t�@�C���f�B�X�N���v�^�͕W������
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