#ifndef MEMORY_MANAGER_H_2019_6_26_
#define MEMORY_MANAGER_H_2019_6_26_

#define FORTH_MEMORY 2000 
#define LOOP_STACK_MEMORY 16
#include<stdio.h>

//多分シングルトンになってる
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

	//if制御文
	void minusIf() {
		Ifnum -= 1;
	}
	void plusIf() {
		Ifnum += 1;
	}
	int getIf() {
		return Ifnum;
	}

	//TODO:Loop制御文
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

		return 0;		//エラーなし
	}

	void push(int);
	void push(float);
	float pop();

private:
	MemoryManager();
	~MemoryManager();

	//シングルトンの実体を指すポインタ
	static MemoryManager* mMemoryInstance;

	//メモリー系のメンバ
	char *mMemory;
	char *mEnd;

	//動かしてもよいポインタ
	char* pCurrent;

	//エラーのフラグを格納する変数
	char err;

	//if文用の値
	char Ifnum;
	//LOOP文用の値
	struct {
		unsigned int loopPoint;
		int loopI;
	} loopStack[LOOP_STACK_MEMORY];
	char nowLoopStack;
};

#endif