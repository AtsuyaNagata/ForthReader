#include "Reader_Lib/XMLReader/Document.h"
#include "FileSystem/File.h"
#include "Reader_Lib/XMLReader/Tag.h"
#include "Reader_Lib/XMLReader/Element.h"

Document::Document() : mRoot(0) {
	mRoot = new Element("<ROOT>");
}

Document::Document(const char* filename) : mRoot(0) {
	//�t�@�C����S���ǂݍ���
	File file(filename);
	//�_�~�[�̍����G�������g�p�Ƀ^�O��p�ӁB���O��<ROOT>
	//<�͖��O�ɂ͎g���Ȃ������Ȃ̂ŁA���ꂪ���ʂł��邱�Ƃ������킩��B
	Tag tag("<ROOT>");
	const char* begin = file.data();
	const char* end = begin + file.size();
	//Element�ɂ͑S�e�L�X�g�̊J�n�n�_�ƏI���n�_��n���ibegin�͕ύX�\�̂��߃|�C���^�œn���j
	mRoot = new Element(&tag, &begin, end);
}

Document::~Document() {
	delete mRoot;
}

void Document::write(const char* filename) const {
	string str;
	//���[�g�v�f�͖������Ă�������
	for (int i = 0; i < mRoot->childNumber(); ++i) {
		//�uchild�v�̏������݊֐������s����ƁA�uchild�v�����L����uchild�v�̏������݊֐����ċA�I�Ɏ��s����
		mRoot->child(i)->convertToString(&str, 0);
	}
	File::write(filename, str.c_str(), static_cast<int>(str.size()));
}

const Element* Document::root() const {
	return mRoot;
}

Element* Document::root() {
	return mRoot;
}