#include "Reader_Lib/XMLReader/Tag.h"
#include "FileSystem/File.h"
#include "Reader_Lib/XMLReader/Attribute.h"

//a-z,A-Z,0-9,_�̂ǂꂩ���𒲂ׂ�֐�
//�킩��₷�����x���B�������̕��@���������Ă݂悤�B
bool static isNormalChar(char c) {
	if (c >= '0' && c <= '9') {
		return true;
	}
	if (c >= 'a' && c <= 'z') {
		return true;
	}
	if (c >= 'A' && c <= 'Z') {
		return true;
	}
	if (c == '_') {
		return true;
	}
	return false;
}

//��ԑJ�ڕ\�B�Œ���Ȃ̂ŁA�G���[�ɂُ͈�ȂقǊ��e�ȍ��ɂȂ��Ă���B
/*
<��Ԕԍ����X�g>
0:<�̏�B�������
1:�G�������g��������
2:�G�������g���̌�̋󔒁B�A�g���r���[�g�̊Ԃ̋󔒁B
3:�A�g���r���[�g��
4:=�̏�
5:�A�g���r���[�g�l(""�̒��g)
E:�I��

<��ԑJ�ڃ��X�g>
�L��:
c = [a-zA-Z0-9_] �܂蕁�ʂ̕���
s = ���̏����Ɉ���������Ȃ��c��̕���

0,/,0 �I���^�O�t���O���Ă�
0,s,1 �G�������g��������ŏ��̈ꕶ��
1,c,1 �G�������g��������ɒǉ�
1,>,E
1,s,2 �G�������g���m��
2,>,E
2,c,3 �A�g���r���[�g���ŏ��̈ꕶ��
2,s,2 �ǂݔ�΂�
3,=,4 =�����B�A�g���r���[�g���m��
3,s,3 �A�g���r���[�g���ɒǉ�
4,",5 �_�u���N�H�e�C�V���������B�A�g���r���[�g�l�Ɉڍs
4,s,4 �ǂݔ�΂�
5,",2 �A�g���r���[�g�l�m��B�A�g���r���[�g�����X�g�ɒǉ��B
5,s,5 �A�g���r���[�g�l������ɒǉ�
*/


//<ROOT>�𐶐�����ۂɗp����
Tag::Tag(const char* name) :
	mName(name),
	mType(TYPE_BEGIN) {
}

Tag::Tag(const char** p, const char* e) :
	mType(TYPE_BEGIN)
{
	//�A�g���r���[�g�̖��O�ƒl�̈ꎞ�I�Ȋi�[�ꏊ
	string name;
	string value;

	int m = 0;			//�V�[�P���X���[�h�B�g�p���������̂ňꕶ��
	bool end = false;	//���[�v�𔲂��邽�߂̕ϐ�
	while (*p < e) {
		char c = **p;	//�ꕶ�����o
		++(*p);			//��i�߂�
		switch (m) {

			case 0:								//�������
				switch (c) {
					case '/':	mType = TYPE_END; break;	//�I���^�O
					default: mName += c; m = 1; break;		//�G�������g���˓�
				}
				break;
			
			case 1:								//�G�������g���̓ǂݎ��
				if (c == '>') {
					end = true;								//�I������
				}
				else if (isNormalChar(c)) {		//���̂��镶����������
					mName += c;					//�������Ԃ�����
				}
				else {
					m = 2;									//�G�������g���̏����I��
				}
				break;

			case 2:								//�G�������g���̌�̋�
				if (c == '>') {
					end = true;
				}
				else if (isNormalChar(c)) {
					name += c;								//�A�g���r���[�g���ǉ�
					m = 3;
				}
				else {
					;										//�������Ȃ�
				}
				break;

			case 3:								//�A�g���r���[�g��
				switch (c) {
					case '=':	m = 4;	break;				//�u=�v�������瑦����
					default:	name += c; break;			//�A�g���r���[�g���ɕ�����ǉ�
				}
				break;

			case 4:											//�u=�v��̏���
				switch (c) {
					case '"':	m = 5;	break;
					default:	break;						//�������Ȃ�
				}
				break;

			case 5:								//�A�g���r���[�g�l
				switch (c) {
					case '"':
						m = 2;
						//�A�g���r���[�g�ǉ�(�ڍׂȂ�)
						mAttributes.push_back(new Attribute( name.c_str(), value.c_str() ) );
						//���O�ƒl��������
						name.clear();
						value.clear();
						break;
						
					default: value += c; break;
				}
				break;
		}
		if (end) {
			break;
		}
	}
}

Tag::~Tag() {
	//������^�O�͕K��Element�̍ޗ��ɂȂ��Ă���͂��ŁA
	//mAttributes�̒��g�͓n���Ă���̂�delete�͂��Ȃ�
}

const string* Tag::name() const {
	return &mName;
}

Tag::Type Tag::type() const {
	return mType;
}

//�d���̂Ń|�C���^�Ԃ�
const vector< Attribute* >* Tag::attributes() const {
	return &mAttributes;
}
