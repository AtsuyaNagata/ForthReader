#ifndef MEMORY_MANAGER_H_2019_6_26_
#define MEMORY_MANAGER_H_2019_6_26_

#define FORTH_MEMORY 2000 
#define LOOP_STACK_MEMORY_SIZE 16
#include<stdio.h>

//�����V���O���g���ɂȂ��Ă�
class MemoryManager
{
public:

	static MemoryManager* instance();
	static void create();
	static void destroy();

	void write(const char* data);
	void writeCurrent(const char* data);
	char* memHead() {
		return mMemory;
	}
	char* readCurrent() {
		return pCurrent;
	}
	bool isStackErr() {
		return 0b0000000001 && err;
	}

	//if���䕶
	void minusIf() {
		Ifnum -= 1;
	}
	void plusIf() {
		Ifnum += 1;
	}
	int getIf() {
		return Ifnum;
	}

	//Loop���䕶
	void pushLoopStack(int nowPoint, int maxPoint, const char* LoopNum) {
		//���[�v�̐�������𒴂�����G���[�������s��
		if (nowLoopStack + 1 >= LOOP_STACK_MEMORY_SIZE) {
			err |= 0b00000100;
			return;
		}
		//�^����ꂽ�l���i�[����
		loopStack[nowLoopStack].loopI = nowPoint;
		loopStack[nowLoopStack].maxI = maxPoint;
		loopStack[nowLoopStack].loopPoint = LoopNum;
		//�X�^�b�N�̒n�_���グ�Ă���
		++nowLoopStack;
	}
	bool checkI() {
		//���̂���ł����鎞�ɂ͕K��nowLoopStack��1�ȏ�̒l�������Ă�
		if (nowLoopStack == 0) {
			err |= 0b00001000;
			return 1;		//Loop����������[����i�����܃M���O�j
		}
		if (loopStack[nowLoopStack - 1].loopI >= loopStack[nowLoopStack - 1].maxI) {	//���[�v�I�����f���s���Ă�
			nowLoopStack -= 1;	//���[�v�̃X�^�b�N��������Ă���
			return 1;			//Loop�I��
		}
		return 0;
	}
	void InclementLoopI() {
		loopStack[nowLoopStack - 1].loopI += 1;
	}
	const char* LoopPoint() {
		return loopStack[nowLoopStack - 1].loopPoint;
	}

	//�G���[�֐�
	void setErr(char e) {
		err |= e;
	}
	bool checkError() {
		if (err & 0b00000001) {
			printf("stack error\n");
			return 1;
		}
		if (err & 0b00000010) {
			printf("then is not found\n");
			return 1;
		}

		return 0;		//�G���[�Ȃ�
	}

	void push(int);
	void push(float);
	float pop();

private:
	MemoryManager();
	~MemoryManager();

	//�V���O���g���̎��̂��w���|�C���^
	static MemoryManager* mMemoryInstance;

	//�������[�n�̃����o
	char *mMemory;
	char *mEnd;

	//�������Ă��悢�|�C���^
	char* pCurrent;

	//�G���[�̃t���O���i�[����ϐ�
	char err;

	//if���p�̒l
	char Ifnum;

	//LOOP���p�̒l
	struct {
		const char* loopPoint;
		int loopI;
		int maxI;
	} loopStack[LOOP_STACK_MEMORY_SIZE];
	//���[�v�̓���q�\���̏��
	char nowLoopStack;
};

#endif