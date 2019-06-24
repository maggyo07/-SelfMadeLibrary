#pragma once

//�V�X�e���n�̃w�b�_�[�C���N���[�h
#include <d3d11.h>
#include <D3DX10math.h>
#include <fstream>
//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h
#include "TextureClass.h"
using namespace std;

class FontClass
{
private:
	//�t�H���g�C���f�b�N�X�t�@�C������ǂݍ��܂ꂽ�C���f�b�N�X�f�[�^�ێ��p
	struct FontType
	{
		float left, right;	//TU�e�N�X�`�����W
		int size;			//�����̕�(�s�N�Z���P��)
	};

	//���_�V�F�[�_�̂��̂ƈ�v
	struct VertexType
	{
		D3DXVECTOR3 possition;
		D3DXVECTOR2 texture;
	};

public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialize(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	//�������������_�����O����O�p�`�̒��_�z���Ԃ�
	//����1	void* vertices	:�Ԃ����_�z��ւ̃|�C���^
	//����2 char* sentence	:�`�悷�镶����
	//����3 float drawX		:����`���ꏊ�̃X�N���[��X���W
	//����4 float drawY		:����`���ꏊ�̃X�N���[��Y���W
	void BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY);

private:
	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	FontType* m_font;
	TextureClass* m_texture;
};