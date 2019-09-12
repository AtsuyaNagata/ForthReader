#ifndef MEMORY_MANAGER_H_2019_6_26_
#define MEMORY_MANAGER_H_2019_6_26_

#define FORTH_MEMORY 2000 
#define LOOP_STACK_MEMORY 16
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

	//TODO:Loop���䕶
	void pushLoopStack(int nowPoint, int LoopNum) {
		if (++nowLoopStack >= LOOP_STACK_MEMORY) {
			return;
		}
	}

	void cleanErr() {
		err = 0b00000000;
	}
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
		unsigned int loopPoint;
		int loopI;
	} loopStack[LOOP_STACK_MEMORY];
	char nowLoopStack;
};

#endif