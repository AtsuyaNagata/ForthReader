#ifndef MEMORY_MANAGER_H_2019_6_26_
#define MEMORY_MANAGER_H_2019_6_26_

//多分シングルトンになってる
class MemoryManager
{
public:
	static MemoryManager* instance();
	static void create();
	static void destroy();

	//4バイトずつプッシュポップする予定
	void push(unsigned char *p);
	void push(int p);
	unsigned pop();

private:
	MemoryManager();
	~MemoryManager();

	//シングルトンの実体を指すポインタ
	static MemoryManager* mMemoryInstance;

	//メモリー系のメンバ
	unsigned char *mMemory;
	unsigned char *mCmdMem;
	unsigned char *mDataMem;
	unsigned char *mEnd;

	//スタックポインタ
	unsigned char* mStack;
	//命令領域のポインタ
	unsigned char* mPC;
};

#endif