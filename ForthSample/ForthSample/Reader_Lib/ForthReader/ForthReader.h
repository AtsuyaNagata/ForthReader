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

	//ファイル読み込みかライン読み込みかを判断する
	void read();

	//解析命令の本体
	void readForth(const char *);
	//コマンドの解析、実行
	int executCmd(const char*);

	~ForthReader();

private:
	//ほんとはこいつが所有すべきではないと思う。あくまでプロトタイプだからね、仕方ないね
	File* mFile;

	ReadStatus mStatus;

	
	struct
	{
		const char* name;
		int const (*func)();
	} forthCmd[MAX_CMDNUMBER];
};

#endif