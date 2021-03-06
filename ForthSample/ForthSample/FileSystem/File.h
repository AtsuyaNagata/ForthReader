#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

class File
{
public:
	File( const char *filename );
	~File();

	int size() const;
	const char* data() const;

	//書き込みstatic関数
	static void write(const char* filename, const char* data, int size);
protected:
	int mSize;
	char* mData;
};

#endif
