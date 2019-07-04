#ifndef MEMORY_MANAGER_H_2019_6_26_
#define MEMORY_MANAGER_H_2019_6_26_

//�����V���O���g���ɂȂ��Ă�
class MemoryManager
{
public:
	static MemoryManager* instance();
	static void create();
	static void destroy();

	//4�o�C�g���v�b�V���|�b�v����\��
	void push(unsigned char *p);
	void push(int p);
	unsigned pop();

private:
	MemoryManager();
	~MemoryManager();

	//�V���O���g���̎��̂��w���|�C���^
	static MemoryManager* mMemoryInstance;

	//�������[�n�̃����o
	unsigned char *mMemory;
	unsigned char *mCmdMem;
	unsigned char *mDataMem;
	unsigned char *mEnd;

	//�X�^�b�N�|�C���^
	unsigned char* mStack;
	//���ߗ̈�̃|�C���^
	unsigned char* mPC;
};

#endif