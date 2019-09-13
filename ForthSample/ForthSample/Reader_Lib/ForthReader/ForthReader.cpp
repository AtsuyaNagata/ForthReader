#include "Reader_Lib/ForthReader/MemoryManager.h"
#include "Reader_Lib/ForthReader/ForthReader.h"
#include "FileSystem/File.h"
#include <iostream>

#define MAX_CMDLENGTH 4

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
//���ꕶ��������}��
bool static isSpCharacter(char c) {
	return ((c == ' ') || (c == '\0') || (c == '\n') || (c == '\t'));
}

//������𐔎��ɕς���}��
double CharacterToNumber(const char* name) {
	double sum = 0;
	//�C���f�b�N�X���w�肷�邽�߂̕ϐ�
	int i, count;

	//�X�e�[�W1 : �����_�ȉ����l�����ɏ���
	for (count = 0; isNumber(name[count]); ++count);

	for (i = 0; i < count; ++i) {
		sum *= 10;
		sum += name[i] - '0';
	}

	//�����A��؂�ꕶ�����u'.'�v�̎��A�����ȉ��̉\������
	if (name[count] == '.') {
		//�X�e�[�W2 : �����_�̒l�̊l��
		i = count + 1;
		for (count = i; isNumber(name[count]); ++count);

		//�W��k��p���ď�����\��
		double k = 0.1;
		for (; i < count; ++i) {
			sum += (name[i] - '0') * k;
			k *= 0.1;
		}
	}
	return sum;
}

//�R�}���h�̏����Q
int const voidCmd();
int const pulusCmd();
int const minusCmd();
int const multipleCmd();
int const divideCmd();
int const printCmd();
int const voidCmd(const char*);
int const ifCmd(const char*);
int const elseCmd(const char*);
int const DoCmd(const char*);
int const LoopCmd(const char*);

//�f�o�b�O�悤���ȁ[�H
ForthReader::ForthReader() :
	mFile(0),
	forthCmd{
		{"+", pulusCmd},
		{"-", minusCmd},
		{"*", multipleCmd},
		{"/", divideCmd},
		{".", printCmd}
	},
	controlCmd{
		{"if", ifCmd},
		{"else", elseCmd},
		{"then", voidCmd},
		{"DO", DoCmd},
		{"LOOP", LoopCmd}
	}
{
	mStatus = READ_LINE;
}

//�{�i�I�ȃR�[�h�ǂݎ��̎��ɗp����\��
ForthReader::ForthReader(const char* name) :
	mFile(0),
	forthCmd{
		{"+", pulusCmd},
		{"-", minusCmd},
		{"*", multipleCmd},
		{"/", divideCmd},
		{".", printCmd}
	},
	controlCmd{
		{"if", ifCmd},
		{"else", elseCmd},
		{"then", voidCmd},
		{"DO", DoCmd},
		{"LOOP", LoopCmd}
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
	//�t�@�C������̓ǂݍ���
	if (mStatus == ForthReader::ReadStatus::READ_FILE) {
		readForth(mFile->data());
	}
	//�W�����͂���̓ǂݍ���
	else if (mStatus == ForthReader::ReadStatus::READ_LINE) {
		//TODO:�ςɂȂ�悤�Ȓ������K�v
		char line[100];
		//�t�@�C���f�B�X�N���v�^�͕W������
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
	int i = 0, j = 0, temp = 0;
	while (p[i] != '\0') {
		if (isNumber(p[i])) {
			mem->push(static_cast<float>(CharacterToNumber(&p[i])));
			for (; !isSpCharacter(p[i]); ++i)
				if (!isNumber(p[i]) && p[i] != '.')		//�s���ȕ��������o����
					mem->pop();							//�s���Ȑ�����f���o��
			for (; !isSpCharacter(p[i]); ++i);			//i�̒l��i�߂Ă���
		}
		else {
			for (int j = 0; j < MAX_CMDLENGTH + 1; ++j) { cmd[j] = '\0'; }	//cmd���N���[������
			for (int j = 0; !isSpCharacter(p[i]); ++i, ++j) {	//cmd��0����Ap��i����n�߂�
				//j���R�}���h�̍ő啶�����𒴂�������ꕶ��������܂�i��i�߂�break������
				if (j >= MAX_CMDLENGTH) {
					for (; !isSpCharacter(p[i]); ++i);
					break;
				}
				cmd[j] = p[i];
			}

			i += executControl(&p[i], cmd);
			if (executCmd(cmd)) {		//�����G���[���������Ă����狭���I��
				return;
			}
		}
		i = i + 1;
	}
}

//�R�}���h�m��A���s����}��
int ForthReader::executCmd(const char* cmdName) {
	//�R�}���h�̐��╶�����͂����܂ő傫���Ȃ��̂�2�d���[�v������
	for (int i = 0; i < MAX_CMD_VALUE; ++i) {		//�萔�����Ƃ����������ǁA�v�����Ȃ�����Ë�
		//printf("cmdName = %s  forthCmd[%d].name = %s\n", cmdName, i, forthCmd[i].name); //�`�F�b�N�p
		if (!strcmp(forthCmd[i].name, cmdName)) {	//��v���������s
			forthCmd[i].func();
			return MemoryManager::instance()->checkError();
		}
	}
	for (int i = 0; i < MAX_CONTROL_VALUE; ++i) {
		if (!strcmp(controlCmd[i].name, cmdName)) {
			return MemoryManager::instance()->checkError();
			return 0;
		}
	}
	printf("%s is not defined", cmdName);
	return 1;
}

int ForthReader::executControl(const char* source, const char* cmdName) {
	for (int i = 0; i < MAX_CONTROL_VALUE; ++i) {
		if (!strcmp(controlCmd[i].name, cmdName)) {
			return controlCmd[i].func(source);
		}
	}
	return 0;
}

//�������Ȃ������ۏ؂��ꂽ�R�}���h�֐�
int const voidCmd() {
	return 0;
}

//�����Z�֐�  1�̓X�^�b�N�G���[
int const pulusCmd() {
	float ans = MemoryManager::instance()->pop() + MemoryManager::instance()->pop();
	if (MemoryManager::instance()->isStackErr()) {
		return 1;		//�X�^�b�N�G���[
	}
	//2�|�b�v���Ĉ�v�b�V��������G���[�̊m�F�͂Ȃ��ł悢
	MemoryManager::instance()->push(ans);
	return 0;
}

//�����Z�֐�
int const minusCmd() {
	float ans = -MemoryManager::instance()->pop() + MemoryManager::instance()->pop();
	if (MemoryManager::instance()->isStackErr()) {
		return 1;		//�X�^�b�N�G���[
	}
	//2�|�b�v���Ĉ�v�b�V��������G���[�̊m�F�͂Ȃ��ł悢
	MemoryManager::instance()->push(ans);
	return 0;
}

//�����Z�֐�
int const multipleCmd() {
	float ans = MemoryManager::instance()->pop() * MemoryManager::instance()->pop();
	if (MemoryManager::instance()->isStackErr()) {
		return 1;		//�X�^�b�N�G���[
	}
	//2�|�b�v���Ĉ�v�b�V��������G���[�̊m�F�͂Ȃ��ł悢
	MemoryManager::instance()->push(ans);
	return 0;
}

//����Z�֐�
int const divideCmd() {
	float divideBuff = MemoryManager::instance()->pop();
	float ans = MemoryManager::instance()->pop() / divideBuff;
	if (MemoryManager::instance()->isStackErr()) {
		return 1;		//�X�^�b�N�G���[
	}
	//2�|�b�v���Ĉ�v�b�V��������G���[�̊m�F�͂Ȃ��ł悢
	MemoryManager::instance()->push(ans);
	return 0;
}

//�o�͊֐�(�X�^�b�N�̈�ԏ���|�b�v���ďo�͂���)
int const printCmd() {
	float printBuff = MemoryManager::instance()->pop();
	if (MemoryManager::instance()->isStackErr()) {
		return 1;		//�X�^�b�N�G���[
	}
	//���s�܂ł��ӔC�Ƃ���
	printf("output : ");
	printf("%f\n", printBuff);
	return 0;
}

int const voidCmd(const char* source) {
	return 0;
}

//source�͐擪�|�C���^�ł͂Ȃ��A���ݓǂ�ł���n�_�̃|�C���^
int const ifCmd(const char* source) {
	int ifBuff = MemoryManager::instance()->pop();
	if (ifBuff) {	//true�̎�
		return 0;
	}
	else {
		for (int i = 0; source[i] != '\0'; ++i) {
			if (source[i] == 't') {
				if (source[i + 1] == 'h') {
					if (source[i + 2] == 'e') {
						if (source[i + 3] == 'n') {
							if (isSpCharacter(source[i + 4])) {
								if (!MemoryManager::instance()->getIf()) {
									return i + 4;
								}
								//then��������l������ׂ�if������������������
								MemoryManager::instance()->minusIf();
							}
						}
					}
				}
			}
			if (source[i] == 'e') {
				if (source[i + 1] == 'l') {
					if (source[i + 2] == 's') {
						if (source[i + 3] == 'e') {
							if (isSpCharacter(source[i + 4])) {
								if (!MemoryManager::instance()->getIf()) {	//�l������if�̐���0��������
									//else�̒�����GOTO����
									return i + 4;
								}
							}
						}
					}
				}
			}
			if (source[i] == 'i') {
				if (source[i + 1] == 'f') {
					//if�̐����v������
					MemoryManager::instance()->plusIf();
				}
			}
		}
		//then�̐�������Ȃ��̂�if�G���[�Ƃ��Ĉ��� (�G���[�r�b�g��2�Ԗڂ̃r�b�g[00000010])
		MemoryManager::instance()->setErr(0b00000010);
		//printf("then is not found\n");
	}

	return 0;
}

int const elseCmd(const char* source) {
	for (int i = 0; source[i] != '\0'; ++i) {
		if (source[i] == 't') {
			if (source[i + 1] == 'h') {
				if (source[i + 2] == 'e') {
					if (source[i + 3] == 'n') {
						if (isSpCharacter(source[i + 4])) {
							return i + 4; 
						}
					}
				}
			}
		}
	}
	//then�̐�������Ȃ��̂�if�G���[�Ƃ��Ĉ���
	MemoryManager::instance()->setErr(0b00000010);
	//printf("then is not found\n");
	return 0;
}

int const DoCmd(const char* source) {
	int startI = MemoryManager::instance()->pop();
	int maxI = MemoryManager::instance()->pop();
	MemoryManager::instance()->pushLoopStack(startI, maxI, source);

	return 0;
}

int const LoopCmd(const char* source) {
	MemoryManager::instance()->InclementLoopI();
	if (MemoryManager::instance()->checkI()) {
		return 0;
	}
	return MemoryManager::instance()->LoopPoint() - source;

	return 0;
}
