#include "Reader_Lib/XMLReader/Element.h"
#include "Reader_Lib/XMLReader/Tag.h"
#include "Reader_Lib/XMLReader/Attribute.h"

Element::Element(){}

Element::Element(const char* name) : mName(name) {}

//�������F�ǂ̃^�O�̃G�������g����낤�Ƃ��Ă��邩	���F�e�L�X�g�f�[�^���|�C���^�Ŏ󂯎��	��O�F�e�L�X�g�̃G���h�|�C���^
Element::Element(Tag* beginTag, const char** p, const char* e)
{
	//���O���ڐA
	mName = *(beginTag->name());
	//�A�g���r���[�g���ڐA(�R�s�[�͂����|�C���^�ŖႤ)
	mAttributes = *beginTag->attributes();

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
	}
}

Element::~Element() {
	for (unsigned i = 0; i < mAttributes.size(); ++i) {
		delete(mAttributes[i]);
		mAttributes[i] = 0;
	}
	for (unsigned i = 0; i < mChildren.size(); ++i) {
		delete[] (mChildren[i]);
		mAttributes[i] = 0;
	}
}

int Element::attributeNumber() const {
	return static_cast< int >(mAttributes.size());
}

Attribute* Element::attribute(int i) {
	return mAttributes[i];
}

void Element::setAttributeNumber(int n) {
	//�����镪�͎̂Ă�
	for (unsigned i = 0; i < mAttributes.size(); ++i) {
		delete[] mAttributes[i];
	}
	mAttributes.resize(n);
	for (int i = 0; i < n; ++i) {
		mAttributes[i] = new Attribute();
	}
}
