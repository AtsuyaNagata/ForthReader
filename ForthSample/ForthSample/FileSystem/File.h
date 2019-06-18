#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

class File
{
public:
	File( const char *filename );
	~File();

	int size() const;
	const char* data() const;

	//‘‚«‚İstaticŠÖ”
	static void write(const char* filename, const char* data, int size);
protected:
	int mSize;
	char* mData;
};

#endif
