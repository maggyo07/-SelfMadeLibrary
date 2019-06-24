#pragma once

//�V�X�e���n�̃w�b�_�[�C���N���[�h
#include <d3d11.h>
#include <D3DX10math.h>
#include <fstream>

//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h
#include "TextureArrayClass.h"

using namespace std;

//3D���f���̃W�I���g�����J�v�Z����
class ModelClass
{
private:
	//���_���(�V�F�[�_�[�̂��̂Ɠ����ɂȂ���΂Ȃ�Ȃ�)
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	};

	struct ModelType
	{
		float x, y, z;		//�ʒu
		float tu, tv;		//�e�N�X�`�����W
		float nx, ny, nz;	//�@��
		float tx, ty, tz;	//
		float bx, by, bz;	//
	};

	//�^���W�F���g�ƃo�C�m�[�}���̌v�Z�Ɏg��
	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();
	//�������֐�
	bool Initialize(WCHAR* modelFilename);
	//�V���b�g�_�E���֐�
	void Shutdown();
	//�t�@�C��������e�N�X�`����ǂݍ���
	bool LoadTexture(WCHAR*);
	//�`��֐�
	void Render(ID3D11DeviceContext*);

	//���f���̐ڐ��x�N�g���Ɠ�l�x�N�g�����v�Z�p
	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType,D3DXVECTOR3&,D3DXVECTOR3&);
	void CalculateNormal(D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3&);

	int GetIndexCount();
	ID3D11ShaderResourceView** GetTextureArray();

private:
	//�������֐�(���f���̓ǂݍ��݁A�o�b�t�@���쐬)
	bool InitalizeBuffers(const WCHAR* modelFilename);
	//�V���b�g�_�E���֐�(�o�b�t�@��p)
	void ShutdownBuffers();
	//�`��֐�(GPU�̓��̓A�Z���u���Œ��_�A�C���f�b�N�X�o�b�t�@���A�N�e�B�u�ɐݒ肷��)
	void RenderBuffers(ID3D11DeviceContext*);


	void ReleaseTexture();

	//�t�@�C�����烂�f������ǂݍ���
	bool LoadModel(const WCHAR* filename, VertexType*& vertexs, unsigned long*& indices);
	void ReleaseModel();
private:
	ID3D11Buffer* m_vertex_buffer;	//���_�o�b�t�@
	ID3D11Buffer* m_index_buffer;	//�C���f�b�N�X�o�b�t�@
	int m_vertex_count;	//���_�̐�
	int m_index_count;	//�C���f�b�N�X�̐�
	TextureArrayClass* m_texture_array;	//�e�N�X�`���f�[�^
	ModelType* m_model;			//���f�����
};