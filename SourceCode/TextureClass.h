#pragma once
//����w�b�_�[�C���N���[�h
#include "TextureShaderClass.h"

//�V�X�e���n�̃w�b�_�[�C���N���[�h
#include <d3d11.h>
#include <D3DX11tex.h>
#include <d3dx9.h>
#include <vector>
#include <memory>

using namespace std;

//�P��̃e�N�X�`���̃��[�h�A�A�����[�h�A����уA�N�Z�X���J�v�Z����
class TextureClass
{

public:
	TextureClass();
	~TextureClass();

	//�������֐�(�t�@�C��������e�N�X�`����ǂݍ���)
	bool Initialize(const WCHAR* file_name);
	//�V���b�g�_�E���֐�
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }
private:
	ID3D11ShaderResourceView* m_texture;	//�V�F�[�_���A�N�Z�X�\�ȃe�N�X�`���f�[�^
	WCHAR* m_texture_file_name;				//�e�N�X�`���̃t�@�C����
	int m_width, m_height;					//�e�N�X�`���̃T�C�Y���
};


class TextureData
{
	//�����p�f�[�^�\����
	struct SplitData
	{
		int all_num;		//�摜�̕�������
		int x_num;			//��������ۂ�X�����ɂ����������邩�̐�
		int y_num;			//��������ۂ�Y�����ɂ����������邩�̐�
		int x_size;			//�������ꂽ1�̑傫��(x)
		int y_size;			//�������ꂽ1�̑傫��(y)
	};
public:
	TextureData(int id);
	~TextureData();
	bool Initialize(const WCHAR* file_name);
	/*�@�����f�[�^��ݒ肷��
	����1 all_num	:�摜�̕�������
	����2 x_num		:��������ۂ�X�����ɂ����������邩�̐�
	����3 y_num		:��������ۂ�Y�����ɂ����������邩�̐�
	����4 x_size	:�������ꂽ1�̑傫��(X)
	����5 y_size	:�������ꂽ1�̑傫��(Y)
	*/
	void SetSplit(int all_num, int x_num, int y_num, int x_size, int y_size);
	/*�����f�[�^�擾�p
	*nullput��ݒ肵�Ă��悢
	�߂�l bool	:true = �����f�[�^������ :false = �����f�[�^������
	*/
	bool GetSplit(int* out_all_num, int* out_x_num, int* out_y_num, int* out_x_size, int* out_y_size);
	TextureClass* GetTextureClass() { return m_texture; }
	int GetID() { return m_id; }
private:
	TextureClass* m_texture;	//�e�N�X�`���f�[�^
	int m_id;					//�o�^ID
	SplitData* m_split_data;	//�摜�𕪊�����Ƃ��Ɏg�p����ϐ�
};

//2D�摜�`��p
class DrawTexture2DClass
{
	//���_���(�V�F�[�_�[�̂��̂Ɠ����ɂȂ���΂Ȃ�Ȃ�)
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
public:
	DrawTexture2DClass();
	bool Initialize(int window_width,int window_height);
	void Delete();
	//�t�@�C��������e�N�X�`����ǂݍ���
	bool LoadTexture(int id, const WCHAR* file_name);
	/*�@�e�N�X�`������o�^
	����3 int	all_num	:�摜�̕�������
	����4 int	x_num	:�摜�̉������ɑ΂��镪����
	����5 int	y_num	:�摜�̏c�����ɑ΂��镪����
	����6 int	x_size	:�������ꂽ1�̑傫��(X)
	����7 int	y_size	:�������ꂽ1�̑傫��(Y)
	�������������ꍇ�A�\�����邽�߂ɕK�v�ȕ���ID�͈ȉ��̂悤�Ɋ���U����
	XNum = 4 ; YNum = 4 ; AllNum = 14 ;
	0	1	2	3
	4	5	6	7
	8	9	10	11
	12	13
	*/
	bool LoadDevTexture(int id, const WCHAR* file_name, int all_num, int x_num, int y_num, int x_size, int y_size);
	void Draw(int texture_id,float pos_x, float pos_y, float size_x, float size_y, float color[4], float r, int dev_id = 0);
	bool Render(int id, float positionX, float positionY, float sizeX, float sizeY, const D3DXCOLOR& color, float r, int dev_id=0);
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture(int id);
private://�֐��p
	//�������֐�(�o�b�t�@��p)
	bool InitalizeBuffers();
	//�폜�֐�(�o�b�t�@��p)
	void DeleteBuffer();
	//���_�o�b�t�@�̓��e���X�V���A�K�v�ɉ�����2D�r�b�g�}�b�v�C���[�W����ʏ�ɍĔz�u
	bool UpdateBuffers(int id, float positionX, float positionY, float sizeX, float sizeY, const D3DXCOLOR& color, float r, int dev_id);
	//�`��֐�(GPU�̓��̓A�Z���u���Œ��_�A�C���f�b�N�X�o�b�t�@���A�N�e�B�u�ɐݒ肷��)
	void RenderBuffers();
	
	//�o�^�����e�N�X�`������j������
	void DeleteTextures();
private://�ϐ��p

	int m_screen_width;	//�E�B���h�E�̕�
	int m_screen_height;	//�E�B���h�E�̍���

	vector<shared_ptr<TextureData>>* m_texture_list;	//�ۊǗp�e�N�X�`���[�f�[�^���X�g

	ID3D11Buffer* m_vertex_buffer;	//���_�o�b�t�@
	ID3D11Buffer* m_index_buffer;	//�C���f�b�N�X�o�b�t�@
	int m_vertex_count;	//���_�̐�
	int m_index_count;	//�C���f�b�N�X�̐�

	TextureShaderClass* m_texture_shader;	//2D�p�e�N�X�`���V�F�[�_�[
};

