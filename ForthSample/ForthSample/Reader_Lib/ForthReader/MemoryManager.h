#ifndef MEMORY_MANAGER_H_2019_6_26_
#define MEMORY_MANAGER_H_2019_6_26_

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
		return mStackErr;
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

	//オーバーフロー、アンダーフローを検知するフラグ
	bool mStackErr;
};

#endif