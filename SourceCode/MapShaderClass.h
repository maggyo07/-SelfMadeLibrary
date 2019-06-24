#pragma once

//�V�X�e���n�̃w�b�_�[�C���N���[�h
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>

using namespace std;

//�}�b�s���O�V�F�[�_�[�N���X
class MapShaderClass
{
private:
	/*�V�F�[�_�[�̃O���[�o��
	���V�F�[�_�[�̂��̂Ɠ����ɂȂ���΂Ȃ�Ȃ�
	���������T�C�Y��16�̔{���łȂ���΂Ȃ�Ȃ�
	*/
	//�s����
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};
	//���C�g���
	struct LightBufferType
	{
		D3DXVECTOR4 diffuse_color;	//���C�g�F���
		D3DXVECTOR3 light_direction;//���C�g�̕���
		float specular_power;		//�X�y�L�����p���[
		D3DXVECTOR4 specular_color;	//�X�y�L�����F���
	};
	//�J�������
	struct CameraBufferType
	{
		D3DXVECTOR3 camera_position;
		float padding;
	};
	//�}�b�s���O�����邩�ǂ����t���O
	struct MapFlagBufferType
	{
		/*
		0x01(����1�o�C�g)	:�o���v�}�b�s���O�t���O
		0x02(����1�o�C�g)	:�X�y�L�����}�b�s���O�t���O
		0x04(����1�o�C�g)	:
		0x08(����1�o�C�g)	:
		*/
		UINT map_flag;
		float padding[3];
	};
public:
	MapShaderClass();
	~MapShaderClass();

	//�������֐�
	bool Initialize(const HWND&);
	//�j���֐�
	void Shutdown();
	//�`��֐�
	bool Render(const int& indexCount, const D3DXMATRIX& worldMatrix,
		const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projectionMatrix, ID3D11ShaderResourceView** texture, const D3DXVECTOR3& light_direction, const D3DXVECTOR4& diffuse_color,
		const D3DXVECTOR3& camera_position, const D3DXVECTOR4& specular_color, const float& specular_power
	);
private://�֐��p
		//�������֐�(�V�F�[�_�t�@�C�������[�h���ADirectX��GPU�Ŏg�p�ł���悤�ɂ���)
		//����1 HWND hwnd				:�E�B���h�E�n���h��
		//����2 WCHAR* vsFilename		:���_�V�F�[�_�R�[�h���i�[����Ă���t�@�C����(�p�X)
		//����3 WCHAR* psFilename		:�s�N�Z���V�F�[�_�R�[�h���i�[����Ă���t�@�C����(�p�X)
	bool InitializeShader(HWND, WCHAR*, WCHAR*);
	//�V���b�g�_�E���֐�(�V�F�[�_�֘A�̃��������)
	void ShutdownShader();
	//�V�F�[�_���R���p�C������Ƃ��ɐ��������G���[���b�Z�[�W���������ފ֐�
	//����1 ID3D10Blob* errorMessage	:�G���[���b�Z�[�W
	//����2 HWND		hwnd			:�E�B���h�E�n���h��
	//����3 WVHAR*		shaderFilename	:�V�F�[�_�t�@�C���̖��O
	void OutputShaderErrorMessage(ID3D10Blob* errorMassage, HWND hwnd, WCHAR* shaderFilename);

	//�V�F�[�_�[���̃O���[�o���ϐ��̐ݒ��e�Ղɂ��邽�߂̊֐�
	bool SetShaderParameters(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projextionMatrix,
		ID3D11ShaderResourceView** texture, D3DXVECTOR3 light_direction, D3DXVECTOR4 diffuse_color,
		const D3DXVECTOR3& camera_position, const D3DXVECTOR4& specular_color, const float& specular_power);

	//���̓A�Z���u���œ��̓��C�A�E�g���A�N�e�B�u�ɐݒ肵�A
	//���_�o�b�t�@�̃����_�����O�Ɏg�p���钸�_�A�s�N�Z���V�F�[�_��ݒ�
	void RenderShader(int indexCount);
private://�ϐ��p
	ID3D11VertexShader* m_vertex_shader;	//���_�V�F�[�_
	ID3D11PixelShader* m_pixel_shader;		//�s�N�Z���V�F�[�_
	ID3D11InputLayout* m_layout;			//���_���̓��C�A�E�g
	ID3D11SamplerState* m_sample_state;		//�e�N�X�`���V�F�[�_�ƃC���^�[�t�F�C�X�Ɏg�p����
	ID3D11Buffer* m_matrix_buffer;			//�s��̒萔�o�b�t�@
	ID3D11Buffer* m_light_buffer;			//���C�g�o�b�t�@
	ID3D11Buffer* m_camera_buffer;			//�J�����o�b�t�@
	ID3D11Buffer* m_map_flag_buffer;		//�}�b�s���O�t���O�o�b�t�@

};