#ifndef READER_LIB_XMLREADER_ATTRIBUTE
#define READER_LIB_XMLREADER_ATTRIBUTE

#include <iostream>
using namespace std;


class Attribute
{
public:
	//�������Ȃ��R���X�g���N�^
	Attribute();
	//������l��p���ăA�g���r���[�g���\������R���X�g���N�^
	Attribute(const char* name, const char* value);
	//�j��_
	~Attribute();

	//�l���i�[����֐��I�[�o�[���[�h�W
	void set(const char* name, const char* value);
	void set(const char* name, int value); //int�l
	void set(const char* name, double value); //double�l
	void set(const char* name, const int* values, int number); //int�z��
	void set(const char* name, const double* values, int number); //double�z��

	const string* value() const;
	const string* name() const;

	//���s�����0��Ԃ����A���Ƃ���0�������Ă����̂Ƃ͋�ʂ����Ȃ��B
	int getIntValue() const;
	//���s�����0.0��Ԃ����A���Ƃ���0.0�������Ă����̂Ƃ͋�ʂ����Ȃ��B
	double getDoubleValue() const;
	//�߂�l�͎��ۂɓǂ߂��v�f��
	int getIntValues(int* out, int number) const;
	//�߂�l�͎��ۂɓǂ߂��v�f��
	int getDoubleValues(double* out, int number) const;

private:
	string mName;
	string mValue;

};

#endif