#ifndef MEMORY_MANAGER_H_2019_6_26_
#define MEMORY_MANAGER_H_2019_6_26_

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
		return mStackErr;
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

	//�I�[�o�[�t���[�A�A���_�[�t���[�����m����t���O
	bool mStackErr;
};

#endif