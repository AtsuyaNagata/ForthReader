#ifndef FORTH_READER_H_2019_7_3_
#define FORTH_READER_H_2019_7_3_

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

	void read();
	void readForth(const char *);

	~ForthReader();

private:
	File* mFile;
	ReadStatus mStatus;
};

#endif