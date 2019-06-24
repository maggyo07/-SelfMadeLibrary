#pragma once

//�V�X�e���n�̃w�b�_�[�C���N���[�h
#include <d3d11.h>
#include <D3DX10math.h>

//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h
#include "TextureClass.h"

using namespace std;

//�O���`
class TextureClass;

//2D�摜�`��p
class BitmapClass
{
private:
	//���_���(�V�F�[�_�[�̂��̂Ɠ����ɂȂ���΂Ȃ�Ȃ�)
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
public:
	BitmapClass();
	BitmapClass(const BitmapClass&);
	~BitmapClass();
	//�������֐�
	bool Initialize(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR* textureFilename, int bitmapWidth, int bitmapHeight);
	//�V���b�g�_�E���֐�
	void Shutdown();
	//�`��֐�
	bool Render(ID3D11DeviceContext*,int,int);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	//�������֐�()
	bool InitalizeBuffers(ID3D11Device*);
	//�V���b�g�_�E���֐�(�o�b�t�@��p)
	void ShutdownBuffers();
	//���_�o�b�t�@�̓��e���X�V���A�K�v�ɉ�����2D�r�b�g�}�b�v�C���[�W����ʏ�ɍĔz�u
	bool UpdateBuffers(ID3D11DeviceContext*, int positionX, int positionY);
	//�`��֐�(GPU�̓��̓A�Z���u���Œ��_�A�C���f�b�N�X�o�b�t�@���A�N�e�B�u�ɐݒ肷��)
	void RenderBuffers(ID3D11DeviceContext*);
	//�t�@�C��������e�N�X�`����ǂݍ���
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer* m_vertex_buffer;	//���_�o�b�t�@
	ID3D11Buffer* m_index_buffer;	//�C���f�b�N�X�o�b�t�@
	int m_vertex_count;	//���_�̐�
	int m_index_count;	//�C���f�b�N�X�̐�
	TextureClass* m_texture;	//�e�N�X�`�����
	int m_screen_width, m_screen_height;	//��ʂ̃T�C�Y
	int m_bitmap_width, m_bitmap_height;	//�r�b�g�}�b�v�̃T�C�Y
	int m_previous_posX,m_previous_posY;	//1�t���[���O�̃����_�����O�ʒu
};