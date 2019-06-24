#pragma once

//�����N
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

//�V�X�e���n�̃w�b�_�[�C���N���[�h
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <D3DX10math.h>

class D3DClass
{
public:
	D3DClass();
	/*�������֐�
	����1 int screenWidth	:�E�B���h�E�̕�
	����2 int screenHeight	:�E�B���h�E�̍���
	����3 bool vsync		:true = �Q�[���̃t���[�����[�g�����j�^�[�̃��t���b�V�����[�g�Ɠ����@false = �ł��邾������
	����4 HWND hwnd			:�E�B���h�E�n���h��
	����5 bool fullscreen	:true = �t���X�N���[���@false = �E�B���h�E���[�h
	����6 float screenDepth	:�����r���[���ʂ�Z�l
	����7 float screenNear	:�߂��r���[���ʂ�Z�l
	*/
	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	//�V���b�g�_�E���֐�
	void Shutdown();
	/*�e�t���[���̎n�߂ɐV����3D�V�[����`�悷���тɌĂяo�����֐�
	����1 float red	:�o�b�t�@���N���A����Ƃ��̐F���@R
	����2 float green	:�o�b�t�@���N���A����Ƃ��̐F���@G
	����3 float bluea	:�o�b�t�@���N���A����Ƃ��̐F���@B
	����4 float alpha	:�o�b�t�@���N���A����Ƃ��̐F���@A*/
	void BeginScene(float, float, float, float);
	//�e�t���[���̍Ō�ɂ��ׂĂ̕`�悪����������3D�V�[����\������悤�ɃX���b�v�`�F�[���Ɏw������֐�
	void EndScene();
	//Direct3D�f�o�C�X�擾(�����_�����O����у��\�[�X�̍쐬�Ɏg�p����)
	ID3D11Device* GetDevice();
	//Direct3D�f�o�C�X�R���e�L�X�g�擾(�����_�����O �R�}���h�𐶐�����f�o�C�X �R���e�L�X�g��\��)
	ID3D11DeviceContext* GetDeviceContext();
	//���e�s����擾
	void GetProjectionMatrix(D3DXMATRIX&);
	//���[���h�����擾
	void GetWorldMatrix(D3DXMATRIX&);
	//���ˉe���e�s��擾
	void GetOrthoMatrix(D3DXMATRIX&);

	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

	//�[�x�X�e���V���r���[�ɃA�N�Z�X�p
	ID3D11DepthStencilView* GetDepthStencilView();
	//�o�b�N�o�b�t�@�����_�[�^�[�Q�b�g���A�N�e�B�u�ɂ���p
	void SetBackBufferRenderTarget();

private:
	bool m_vsync_enabled;							//true = �Q�[���̃t���[�����[�g�����j�^�[�̃��t���b�V�����[�g�Ɠ����@false = �ł��邾������
	int m_video_card_memory;						//��p�̃r�f�I�J�[�h�̃�����(���K�o�C�g�P��)
	char m_video_card_description[128];				//�r�f�I�J�[�h�̖��O
	IDXGISwapChain* m_swap_chain;					//�X���b�v�`�F�[��(�t�����g�o�b�t�@�ƃo�b�N�o�b�t�@���g���āA��ʂ��X�V����d�g�݁B)
	ID3D11Device* m_device;							//Direct3D�f�o�C�X
	ID3D11DeviceContext* m_device_context;			//Direct3D�f�o�C�X�R���e�L�X�g
	ID3D11RenderTargetView* m_render_target_view;	//�����_�[�^�[�Q�b�g�r���[
	ID3D11Texture2D* m_depth_stencil_buffer;		//�[�x�o�b�t�@�̃e�N�X�`��
	ID3D11DepthStencilState* m_depth_stencil_state;	//�[�x�X�e���V�����
	ID3D11DepthStencilState* m_depth_disabled_stencil_state;//�[�x�X�e���V�����(Z�o�b�t�@OFF)
	ID3D11DepthStencilView* m_depth_stencil_view;	//�[�x�X�e���V�� �r���[����A�N�Z�X�\�ȃe�N�X�`���[�̃T�u���\�[�X
	ID3D11RasterizerState* m_raster_state;			//���X�^���C�U�[ �X�e�[�g(�ǂ̂悤�Ƀ|���S����`�悷�邩�����肷�郉�X�^�L�q)
	D3DXMATRIX m_projection_matrix;					//���e�s��B(3D�V�[�����ȑO�ɍ쐬����2D�r���[�|�[�g��Ԃɕϊ����邽��)
	D3DXMATRIX m_world_matrix;						//���[���h�s��
	D3DXMATRIX m_ortho_matrix;						//���ˉe���e�s��B(2D�v�f����ʏ�ɕ`�悷�邽��)
	ID3D11BlendState* m_alpha_enable_blending_state;//�A���t�@�u�����f�B���OON�p�̃u�����h���
	ID3D11BlendState* m_alpha_disable_blending_state;//�A���t�@�u�����f�B���OOFF�p�̃u�����h���
};
