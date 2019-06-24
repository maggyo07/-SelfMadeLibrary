#pragma once

//�V�X�e���n�̃w�b�_�[�C���N���[�h
#include <d3d11.h>

//����w�b�_�[�C���N���[�h
#include "D3DClass.h"
/*
RenderTextureClass���g�p����ƁA
�o�b�N�o�b�t�@�ł͂Ȃ������_�����O�^�[�Q�b�g�Ƃ��Đݒ�ł��܂��B
�܂��AID3D11ShaderResourceView�̌`���ł���Ƀ����_�����O���ꂽ�f�[�^���擾���邱�Ƃ��ł��܂��B
*/
class RenderTextureClass
{
public:
	RenderTextureClass();
	~RenderTextureClass();

	/* �������֐�
	����1 texture_width	:�����_�����O���e�N�X�`���ɂ�������
	����2 texture_height:�����_�����O���e�N�X�`���ɂ���������
	����ʂ������_�����O���Ă���ꍇ�́A�e�N�X�`���ɑ΂��邱�̃����_�����O��
	�@�c�������ʂ̏c����Ɠ����ɂ���K�v������B�łȂ��ƁA�T�C�Y�̂䂪�݂���������
	 �����F
	 ���̊֐��́A�ŏ��Ƀe�N�X�`���̐�����ݒ肵�Ă��炻�̃e�N�X�`�����쐬���邱�Ƃɂ���āA
	 �����_�[�^�[�Q�b�g�e�N�X�`�����쐬���܂��B
	 ���ɁA���̃e�N�X�`�����g�p���ă����_�[�^�[�Q�b�g�r���[��ݒ肵�A�e�N�X�`���������_�[�^�[�Q�b�g�Ƃ��ĕ`��ł���悤�ɂ��܂��B
	 �Ō�ɍs�����Ƃ́A�����_�����O���ꂽ�f�[�^���Ăяo�����̃I�u�W�F�N�g�ɒ񋟂ł���悤�ɁA
	 ���̃e�N�X�`���̃V�F�[�_���\�[�X�r���[���쐬���邱�Ƃł��B
	 */
	bool Initialize(const int& texture_width, const int& texture_height);
	void Shutdown();

	/*
	SetRenderTarget�֐��́A���̃N���X�̃����_�����O�^�[�Q�b�g�r���[���A
	�����_�����O��̂��ׂẴO���t�B�b�N�̌��݂̃����_�����O�ʒu�Ƃ��Đݒ肵�܂��B
	*/
	void SetRenderTarget();
	/*
	ClearRenderTarget�́A���̃N���X���̃����_�[�^�[�Q�b�g�r���[�ɑ΂��ċ@�\���邱�Ƃ������āA
	D3DClass :: BeginScene�֐��̋@�\��͕킵�Ă��܂��B
	���̃����_�[�^�[�Q�b�g�r���[�Ƀ����_�����O����O�ɁA
	������e�t���[���ƌĂт܂��B
	*/
	void ClearRenderTarget(const D3DXCOLOR& color);
	/*
	GetShaderResourceView�֐��́A�����_�����O���V�F�[�_���\�[�X�r���[�Ƃ��ăe�N�X�`���f�[�^�ɕԂ��܂��B
	���̕��@�ł́A�����_�[�^�[�Q�b�g�r���[�Ƀ����_�����O���ꂽ���̂͂��ׂāA
	���̊֐����Ăяo�����܂��܂ȃV�F�[�_�Ńe�N�X�`���Ƃ��Ďg�p�ł��܂��B
	�ʏ�A�e�N�X�`�����V�F�[�_�ɑ��M����ꏊ�ł́A����ɂ��̊֐��ւ̌Ăяo�������ɑ��M���邱�Ƃ��ł��A
	�e�N�X�`���ւ̃����_�����O���g�p����܂��B
	*/
	ID3D11ShaderResourceView* GetShaderResourceView();
private:
	ID3D11Texture2D* m_render_rarget_texture;			//�����_�����O�^�[�Q�b�g�e�N�X�`��
	ID3D11RenderTargetView* m_render_target_view;		//�����_�����O�^�[�Q�b�g�r���[
	ID3D11ShaderResourceView* m_shader_resource_view;	//�V�F�[�_�[���\�[�X�r���[
};