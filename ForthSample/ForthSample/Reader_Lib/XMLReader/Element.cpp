#include "Reader_Lib/XMLReader/Element.h"
#include "Reader_Lib/XMLReader/Tag.h"
#include "Reader_Lib/XMLReader/Attribute.h"

Element::Element(){}

Element::Element(const char* name) : mName(name) {}

//�������F�ǂ̃^�O�̃G�������g����낤�Ƃ��Ă��邩	���F�e�L�X�g�f�[�^���|�C���^�Ŏ󂯎��	��O�F�e�L�X�g�̃G���h�|�C���^
Element::Element(Tag* beginTag, const char** p, const char* e)
{
	//���O���ڐA�F<ROOT>�������Ă������
	mName = *(beginTag->name());
	//�A�g���r���[�g���ڐA(���g�̃R�s�[�͂����|�C���^�ŖႤ)  �J����Element�̎d��
	mAttributes = *beginTag->attributes();		//�|�C���^�̒l���ۂ��ƃR�s�[

	//�^�O�T��
	while (*p < e) {
		if (**p == '<') {	//�q�ɂȂ肻���Ȃ�𔭌��I
			++(*p);			//�u<�v�̎��ֈړ�
			Tag tag(p, e);	//p����e�܂ł̂̃^�O��͂��ۓ���
			Tag::Type type = tag.type();
			if (type == Tag::TYPE_BEGIN) {	//�v�f�� </Element> ��������q�ł͂Ȃ�
				mChildren.push_back(new Element(&tag, p, e));	//�q�ɒǉ�
			}
			else if (type == Tag::TYPE_END) {
				++(*p);		//�u<�v������܂Ŗ�����������I
			}
		}
		else {
			++(*p); //<���o��܂Ŗ������܂���
		}
	}
}

//�f�X�g���N�^
Element::~Element() {
	for (unsigned i = 0; i < mAttributes.size(); ++i) {
		delete mAttributes[i];
	}
	for (unsigned i = 0; i < mChildren.size(); ++i) {
		delete mChildren[i];
	}
}

//�A�g���r���[�g�T�C�Y����ɓ����
int Element::attributeNumber() const {
	//size_type�^����int�^��	(size_type�^�͗v�f�𐔂���̂��p�Ƃ������̂ŕ���������)
	return static_cast< int >(mAttributes.size());
}

//[i]�Ԗڂ̃A�g���r���[�g���l��
Attribute* Element::attribute(int i) {
	return mAttributes[i];
}

//�V����n�̃A�g���r���[�g�����
void Element::setAttributeNumber(int n) {
	//�����镪�͎̂Ă�
	for (unsigned i = 0; i < mAttributes.size(); ++i) {
		delete mAttributes[i];
	}
	mAttributes.resize(n);
	for (int i = 0; i < n; ++i) {
		mAttributes[i] = new Attribute();
	}
}

//�q���̐����l��
int Element::childNumber() const {
	//size_type�^����int�^��
	return static_cast<int>(mChildren.size());
}

//[i]�Ԗڂ̎q�����l��
const Element* Element::child(int i) const {
	return mChildren[i];
}

//���g��������������
Element* Element::child(int i) {
	return mChildren[i];
}

//�V���Ȏq���𐶐��i���܂ł̎q���͏�����̂ł������炸�j
void Element::setChildNumber(int n) {
	//�����镪�͎̂Ă�
	for (unsigned i = 0; i < mChildren.size(); ++i) {
		delete mChildren[i];
	}
	mChildren.resize(n);
	for (int i = 0; i < n; ++i) {
		mChildren[i] = new Element();
	}
}

//���O��Ԃ��B�|�C���^��
const string* Element::name() const {
	return &mName;
}

//���O�̏�������
void Element::setName(const char* name) {
	mName = name;
}

//�������݂��s��(</Name>�̂��߂̃C���f���g)�ċA�֐�
void Element::convertToString(string* out, int indent) const {
	//�C���f���g�̐������^�u������	(<Name>�̂��߂̃C���f���g)
	for (int i = 0; i < indent; ++i) {
		*out += '\t';
	}
	//�^�O�J�n�ƃG�������g��
	*out += '<';
	*out += mName;
	//�A�g���r���[�g��������
	for (unsigned i = 0; i < mAttributes.size(); ++i) {
		if (mAttributes[i]) {
			*out += ' '; //�X�y�[�X
			*out += *(mAttributes[i]->name());	//�J�n�^�O���̃A�g���r���[�g�W
			*out += "=\"";						//�u="�v���L��
			*out += *(mAttributes[i]->value());	//�l������
			*out += '"';
		}
	}
	*out += ">\r\n"; //�J�n�^�O�I���
	//�q�ɗ���
	for (unsigned i = 0; i < mChildren.size(); ++i) {
		//�q�̗v�f�������
		if (mChildren[i]) {
			//�q�̃G�������g��W�J���Ă����B���̎��A�C���f���g�������邱�Ƃł���������
			mChildren[i]->convertToString(out, indent + 1);
		}
	}
	//�C���f���g�̐������^�u������	(</Name>�̂��߂̃C���f���g)
	for (int i = 0; i < indent; ++i) {
		*out += '\t';
	}
	//�I���^�O
	*out += "</";
	*out += mName;
	*out += ">\r\n";
}
