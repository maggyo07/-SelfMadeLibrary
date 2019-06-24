#pragma once
//�V�X�e���n�̃w�b�_�[�C���N���[�h
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>


using namespace std;

//HLSL�V�F�[�_���Ăяo�����߂̃N���X
class FontShaderClass
{
	private:	//�V�F�[�_�[�̃O���[�o��(�V�F�[�_�[�̂��̂Ɠ����ɂȂ���΂Ȃ�Ȃ�)
		//�s����
		struct MatrixBufferType
		{
			D3DXMATRIX world;
			D3DXMATRIX view;
			D3DXMATRIX projection;
		};

		//�����_�����O�����e�L�X�g�̃s�N�Z���F
		struct PixelBufferType
		{
			D3DXVECTOR4 pixelColor;
		};
public:
	FontShaderClass();
	FontShaderClass(const FontShaderClass&);
	~FontShaderClass();
	
	//�������֐�
	bool Initialize(ID3D11Device*, HWND);
	//�V���b�g�_�E���֐�
	void Shutdown();
	//�`��֐�
	bool Render(ID3D11DeviceContext*, int indexCount, D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,ID3D11ShaderResourceView* texture,
		D3DXVECTOR4 pixelColor);

private:
	//�������֐�(�V�F�[�_�t�@�C�������[�h���ADirectX��GPU�Ŏg�p�ł���悤�ɂ���)
	//����1 ID3D11Device* device	:�f�o�C�X�̃|�C���^
	//����2 HWND hwnd				:�E�B���h�E�n���h��
	//����3 WCHAR* vsFilename		:���_�V�F�[�_�R�[�h���i�[����Ă���t�@�C����(�p�X)
	//����4 WCHAR* psFilename		:�s�N�Z���V�F�[�_�R�[�h���i�[����Ă���t�@�C����(�p�X)
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	//�V���b�g�_�E���֐�(�V�F�[�_�֘A�̃��������)
	void ShutdownShader();
	//�V�F�[�_���R���p�C������Ƃ��ɐ��������G���[���b�Z�[�W���������ފ֐�
	//����1 ID3D10Blob* errorMessage	:�G���[���b�Z�[�W
	//����2 HWND		hwnd			:�E�B���h�E�n���h��
	//����3 WVHAR*		shaderFilename	:�V�F�[�_�t�@�C���̖��O
	void OutputShaderErrorMessage(ID3D10Blob* errorMassage, HWND hwnd, WCHAR* shaderFilename);

	//�V�F�[�_�[���̃O���[�o���ϐ��̐ݒ��e�Ղɂ��邽�߂̊֐�
	bool SetShaderVariables(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix, D3DXMATRIX projextionMatrix,ID3D11ShaderResourceView* texture,
		D3DXVECTOR4 pixelColor);

	//���̓A�Z���u���œ��̓��C�A�E�g���A�N�e�B�u�ɐݒ肵�A
	//���_�o�b�t�@�̃����_�����O�Ɏg�p���钸�_�A�s�N�Z���V�F�[�_��ݒ�
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_vertex_shader;	//���_�V�F�[�_
	ID3D11PixelShader* m_pixel_shader;		//�s�N�Z���V�F�[�_
	ID3D11InputLayout* m_layout;			//���_���̓��C�A�E�g
	ID3D11SamplerState* m_sample_state;		//�e�N�X�`���V�F�[�_�ƃC���^�[�t�F�C�X�Ɏg�p����
	ID3D11Buffer* m_matrix_buffer;			//�s��̒萔�o�b�t�@
	ID3D11Buffer* m_pixel_buffer;			//�s�N�Z���J���[�萔�o�b�t�@
};