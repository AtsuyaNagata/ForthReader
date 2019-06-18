#ifndef READER_LIB_XMLREADER_ATTRIBUTE
#define READER_LIB_XMLREADER_ATTRIBUTE

#include <iostream>
using namespace std;


class Attribute
{
public:
	//何もしないコンストラクタ
	Attribute();
	//貰った値を用いてアトリビュートを構成するコンストラクタ
	Attribute(const char* name, const char* value);
	//破壊神
	~Attribute();

	//値を格納する関数オーバーロード集
	void set(const char* name, const char* value);
	void set(const char* name, int value); //int値
	void set(const char* name, double value); //double値
	void set(const char* name, const int* values, int number); //int配列
	void set(const char* name, const double* values, int number); //double配列

	const string* value() const;
	const string* name() const;

	//失敗すると0を返すが、もともと0が入っていたのとは区別がつかない。
	int getIntValue() const;
	//失敗すると0.0を返すが、もともと0.0が入っていたのとは区別がつかない。
	double getDoubleValue() const;
	//戻り値は実際に読めた要素数
	int getIntValues(int* out, int number) const;
	//戻り値は実際に読めた要素数
	int getDoubleValues(double* out, int number) const;

private:
	string mName;
	string mValue;

};

#endif