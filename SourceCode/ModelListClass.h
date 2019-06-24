#pragma once

#include <D3DX10math.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <list>
#include <memory>

using namespace std;

constexpr auto SEARCH_LENGTH_MAX{ 50 };

class ModelListClass
{
private:
	struct ModelInfoType
	{
		D3DXVECTOR4 color;
		D3DXVECTOR3 position;
	};
public:
	ModelListClass();
	bool Initialize();
	void Delete();

	int GetModelCount();
	void GetData(int index, D3DXVECTOR3& position, D3DXVECTOR4& color);
	void TestRandomBllCreation(int num);	//�����_���Ȉʒu�ƐF�̋ʂ��쐬����(�e�X�g�p)
	bool LoadModel(const wchar_t*);
	/*
	�ǂݍ��ݍς݃t�@�C������w����������T�����A�����܂œǂݍ���
	����1 fin			:�ǂݍ��ݍς݃t�@�C��
	����2 search_str	:�T��������(���̕����񂪂��邩�ǂ�����T������)
	�߂�l bool	:true������	false�����s
	*/
	bool StringSearchFromFile(ifstream& fin, const char* search_str);
private:
	list<shared_ptr<ModelInfoType>>* m_model_list;

};