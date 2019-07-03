#ifndef FORTH_READER_H_2019_7_3_
#define FORTH_READER_H_2019_7_3_

class File;

class ForthReader
{
public:
	ForthReader();
	~ForthReader();

	enum ReadStatus {
		READ_LINE,
		READ_FILE,

		READ_NONE
	};

private:
	File* mFile;
	ReadStatus mStatus;
};

#endif