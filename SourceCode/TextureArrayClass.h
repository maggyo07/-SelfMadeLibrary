#pragma once

//�V�X�e���n�̃w�b�_�[�C���N���[�h
#include <d3d11.h>
#include <D3DX11tex.h>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;



//�����̃e�N�X�`���ǂݍ��ݗp�N���X
class TextureArrayClass
{
	struct Texture
	{
	public:
		Texture();
		~Texture();
		bool Initialize(const WCHAR* file_name);
		ID3D11ShaderResourceView* GetTexture();
	private:
		ID3D11ShaderResourceView* m_texture;//�e�N�X�`���f�[�^
	};
public:
	TextureArrayClass();
	~TextureArrayClass();
	//�e�N�X�`����ǂݍ���
	bool LoadTexture(const WCHAR* file_name);

	//�e�N�X�`���̔z��̐擪�|�C���^��Ԃ�
	//���K���g���I�������delete[]���Ă��������B
	ID3D11ShaderResourceView** GetTextureArray(int* out_count);
public:
	//�e�N�X�`������
	vector<shared_ptr<Texture>>* m_textures;

};