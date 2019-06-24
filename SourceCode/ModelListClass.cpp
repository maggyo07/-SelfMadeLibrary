#include "ModelListClass.h"

#include <fstream>

ModelListClass::ModelListClass()	:m_model_list(new list<shared_ptr< ModelInfoType >>)
{

}
bool ModelListClass::Initialize()
{
	return true;
}

void ModelListClass::Delete()
{
	if (m_model_list)
	{
		m_model_list->clear();
		delete m_model_list;
	}
}

int ModelListClass::GetModelCount()
{
	return m_model_list->size();
}

void ModelListClass::GetData(int index,D3DXVECTOR3& position, D3DXVECTOR4& color)
{
	//index�̒l����������
	if (index < 0 || index >= m_model_list->size())
		return;

	auto itr = m_model_list->begin();
	for (; index != 0;index--)
		itr++;

	if (position != nullptr)
		position = itr->get()->position;

	if (color != nullptr)
		color = itr->get()->color;
}
//�����_���Ȉʒu�ƐF�̋ʂ��쐬����(�e�X�g�p)
void ModelListClass::TestRandomBllCreation(int num)
{
	//�����_���W�F�l���[�^�Ɍ��݂̎������V�[�h����
	srand((unsigned int)time(NULL));

	//�w�萔�����쐬����
	for (int count = 0; count < num; count++)
	{
		//���X�g�ɒǉ�����ϐ�
		shared_ptr<ModelInfoType> add_model_info(new ModelInfoType);

		//���f���̐F�������_���ɐ�������
		add_model_info.get()->color.x = (float)rand() / RAND_MAX;	//��
		add_model_info.get()->color.y = (float)rand() / RAND_MAX;	//��
		add_model_info.get()->color.z = (float)rand() / RAND_MAX;	//��
		add_model_info.get()->color.w = 1.0f;						//����

		//���[�h�ɑ΂��ăr���[�A�̑O�Ƀ����_���Ȉʒu�𐶐�����
		add_model_info.get()->position.x = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		add_model_info.get()->position.y = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		add_model_info.get()->position.z = ((((float)rand() - (float)rand()) / RAND_MAX) * 10.0f) + 0.5f;

		//���X�g�ɒǉ�
		m_model_list->push_back(move(add_model_info));
	}		 
}

bool ModelListClass::LoadModel(const wchar_t* name)
{
	//�J�����E�E�E

	//�Q�ƃT�C�g
	//https://qiita.com/gshirato/items/87586421de0ec37e46a6
	//http://help.autodesk.com/view/FBX/2017/ENU/?guid=__files_GUID_274163DA_9E89_4DCC_8AF6_10B0C498582E_htm
	//https://hexed.it/
	//�t�@�C�����J���p
	ifstream fin;

	//�t�@�C�����J��
	fin.open(name,ios::binary | ios::in);

	//�t�@�C����������Ȃ���ΏI��
	if (fin.fail())
		return false;
	
	//�w�b�_�[��ǂݍ���(�ŏ���27�o�C�g)-----------------
	char magic_number[21];	//�}�W�b�N�i���o�[
	char padding[2];		//�g���݂��͕s�������S�Ă̊m�F�ς݃t�@�C���ł��̕���
	UINT fbx_version;			//�o�[�W����
	fin.read(magic_number, sizeof(magic_number));
	fin.read(padding, sizeof(padding));
	fin.read((char*)&fbx_version, sizeof(fbx_version));

	//�}�W�b�N�i���o�[�����ăo�C�i���f�[�^���ǂ������m�F
	if (!strstr(magic_number, "Binary"))
		return false;
	//---------------------------------------------------
	//-----------FBXHeaderExtension----------------------
	if (StringSearchFromFile(fin, "FBXHeaderExtension"))
	{
		//�w�b�_�[�o�[�W����
		UINT heder_version;
		//�o�[�W����
		UINT version;
		if (StringSearchFromFile(fin, "FBXHeaderVersionI"))
			fin.read((char*)&heder_version, sizeof(heder_version));
		if (StringSearchFromFile(fin, "FBXVersionI"))
			fin.read((char*)&version, sizeof(version));
		if (StringSearchFromFile(fin, "CreationTimeStamp"))
		{

		}
	}
	

	//---------------------------------------------------
	char aa = 0x49;

	char xx[100];
	fin.read(xx, 100);
	char c;
	while (fin.get(c))
	{
		c;
	}

	fin.close();
	return true;
}

/*
�ǂݍ��ݍς݃t�@�C������w����������T�����A�����܂œǂݍ���
����1 fin			:�ǂݍ��ݍς݃t�@�C��
����2 search_str	:�T��������(���̕����񂪂��邩�ǂ�����T������)
�߂�l bool	:true������	false�����s
*/
bool ModelListClass::StringSearchFromFile(ifstream& fin, const char* search_str)
{
	//�ǂݍ��܂�Ă��Ȃ�
	if (!fin)
		return false;
	//�t�@�C������擾���镶����i�[�p(��x�Ɏ��镶����50)
	char fin_str[SEARCH_LENGTH_MAX];
	
	//����
	int length = strlen(search_str);

	//�T��������̐����ő�l�𒴂��Ă�����A
	//�O�ȉ��ł���ΏI��
	if (length > SEARCH_LENGTH_MAX || length <= 0)
		return false;

	//
	while (fin.get(fin_str[0]))
	{
		if (fin_str[0] == search_str[0])
		{
			fin.read(&fin_str[1], length - 1);

			if (strstr(fin_str, search_str))
				return true;
		}
	}

	return false;
}
