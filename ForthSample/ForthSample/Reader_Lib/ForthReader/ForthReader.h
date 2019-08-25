#ifndef FORTH_READER_H_2019_7_3_
#define FORTH_READER_H_2019_7_3_

#define MAX_CMDNUMBER 8

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

	~ForthReader();

private:
	//�ق�Ƃ͂��������L���ׂ��ł͂Ȃ��Ǝv���B�����܂Ńv���g�^�C�v������ˁA�d���Ȃ���
	File* mFile;

	ReadStatus mStatus;

	
	struct
	{
		const char* name;
		int const (*func)();
	} forthCmd[MAX_CMDNUMBER];
};

#endif