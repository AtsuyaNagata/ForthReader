#include "Reader_Lib/XMLReader/Document.h"
#include "FileSystem/File.h"

Document::Document(const char* filename)
{
	//�t�@�C�������ׂēǂ�
	File file(filename);

	//�_�~�[�̍��{�G�������g�p�Ƀ^�O��p�ӂ���B��B
	Tag tag("Dummy");
	//�e�L�X�g���߂͍s�킸�A�ۓ���
	mRoot = new Element(&tag, file.data());
}