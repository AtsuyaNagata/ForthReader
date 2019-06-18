#ifndef READER_LIB_XMLREADER_TAG
#define READER_LIB_XMLREADER_TAG

#include <iostream>
#include <vector>
using namespace std;
class Element;
class Attribute;

class Tag
{
public:
	enum Type{
		TYPE_BEGIN,
		TYPE_END
	};
	Tag(const char* name); //���O�����̃^�O�����
	Tag(const char** readPointer, const char* end);
	~Tag();
	const string* name() const;
	//�A�g���r���[�g�z��Q�b�g�B�d������|�C���^�Ԃ��B
	const vector< Attribute* >* attributes() const;
	Type type() const;

private:
	vector< Attribute* > mAttributes;	//�^�O�����L����ϐ��I�ȕ�
	string mName;						//�^�O�̖��O
	Type mType;							//�J�n�^�O���I���^�O��
};


/*
Tag�N���X����͂��s���AElement��Attribute���\�z����.
Element�͊J�n�^�O�ƏI���^�O�ɋ��܂�Ă���A���̊ԂɃ^�O����������q�v�f�Ƃ��ċz������.
�܂�AElement��Element�����L����\��������.
���̏��AElement��Tag���������ł��Ȃ�.
*/

#endif