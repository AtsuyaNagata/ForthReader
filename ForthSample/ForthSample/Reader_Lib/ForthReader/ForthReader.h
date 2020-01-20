#ifndef FORTH_READER_H_2019_7_3_
#define FORTH_READER_H_2019_7_3_

#define MAX_CMD_VALUE 9			//�R�}���h��
#define MAX_CONTROL_VALUE 5		//���䕶�̐�

class File;

class ForthReader
{
public:
	enum ReadStatus {
		READ_LINE,
		READ_FILE,

		READ_NONE
	};

	ForthReader();
	ForthReader(const char *p);

	//�t�@�C���ǂݍ��݂����C���ǂݍ��݂��𔻒f����
	void read();

	//��͖��߂̖{��
	void readForth(const char *);
	//�R�}���h�̉�́A���s
	int executCmd(const char*);
	//���䕶�̉�́A���s
	int executControl(const char*, const char*);

	~ForthReader();

private:
	File* mFile;

	ReadStatus mStatus;

	
	struct
	{
		const char* name;
		void (* const func)();
	} forthCmd[MAX_CMD_VALUE];
	struct
	{
		const char* name;
		int const (* const func)(const char* cmdName);
	} controlCmd[MAX_CONTROL_VALUE];
};

#endif