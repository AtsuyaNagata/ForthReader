#ifndef MEMORY_MANAGER_H_2019_6_26_
#define MEMORY_MANAGER_H_2019_6_26_

#define FORTH_MEMORY 2048
#define LOOP_STACK_MEMORY_SIZE 8
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

	float* getVariableMemory() {
		return mVariable;
	}

	//Loop制御文
	void pushLoopStack(int nowPoint, int maxPoint, const char* LoopNum) {
		//ループの数が上限を超えたらエラー処理を行う
		if (nowLoopStack + 1 >= LOOP_STACK_MEMORY_SIZE) {
			err |= 0b00000100;
			return;
		}
		//与えられた値を格納する
		loopStack[nowLoopStack].loopI = nowPoint;
		loopStack[nowLoopStack].maxI = maxPoint;
		loopStack[nowLoopStack].loopPoint = LoopNum;
		//スタックの地点を上げておく
		++nowLoopStack;
	}
	bool checkI() {
		//実体が一つでもある時には必ずnowLoopStackは1以上の値を持ってる
		if (nowLoopStack == 0) {
			err |= 0b00001000;
			return 1;		//Loop処理をするーする（激うまギャグ）
		}
		if (loopStack[nowLoopStack - 1].loopI >= loopStack[nowLoopStack - 1].maxI) {	//ループ終了判断を行ってる
			nowLoopStack -= 1;	//ループのスタックを一つ下げておく
			return 1;			//Loop終了
		}
		return 0;
	}
	void InclementLoopI() {
		loopStack[nowLoopStack - 1].loopI += 1;
	}
	const char* LoopPoint() {
		return loopStack[nowLoopStack - 1].loopPoint;
	}

	//エラー関数
	void setErr(char e) {
		err |= e;
	}
	bool checkError() {
		int errNum = 0;
		if (err & 0b00000001) {
			printf("stack error\n");
			errNum = 1;
		}
		if (err & 0b00000010) {
			printf("then is not found\n");
			errNum = 1;
		}

		return errNum;		//0:エラーなし
	}

	void push(int);
	void push(float);
	float pop();

private:
	MemoryManager();
	~MemoryManager();

	//シングルトンの実体を指すポインタ
	static MemoryManager* mMemoryInstance;

	//スタックメモリー系のメンバ
	char *mMemory;
	char *mEnd;

	//変数領域
	float* mVariable;

	//動かしてもよいポインタ
	char* pCurrent;

	//エラーのフラグを格納する変数
	char err;

	//LOOP文用の値
	struct {
		const char* loopPoint;
		int loopI;
		int maxI;
	} loopStack[LOOP_STACK_MEMORY_SIZE];
	//ループの入れ子構造の上限
	char nowLoopStack;
};

#endif