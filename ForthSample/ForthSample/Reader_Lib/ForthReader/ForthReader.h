#ifndef FORTH_READER_H_2019_7_3_
#define FORTH_READER_H_2019_7_3_

#define MAX_CMD_VALUE 9			//コマンド数
#define MAX_CONTROL_VALUE 5		//制御文の数

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
	//制御文の解析、実行
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