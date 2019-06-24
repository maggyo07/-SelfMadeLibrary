#pragma once

//�V�X�e���n�̃w�b�_�[�C���N���[�h
#include <Windows.h>
//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h
#include "D3DClass.h"
#include "TextureClass.h"
#include "ModelListClass.h"

#include "FrustumClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ShaderClass.h"
#include "LightClass.h"
#include "TextureShaderClass.h"
#include "TextureClass.h"
#include "TextClass.h"
#include "MultiTextureShaderClass.h"
#include "BumpMapShaderClass.h"
#include "MapShaderClass.h"
#include "RenderTextureClass.h"

//�t���X�N���[����Ԃ��ۂ�
constexpr bool FULL_SCREEN = false;
//true = �Q�[���̃t���[�����[�g�����j�^�[�̃��t���b�V�����[�g�Ɠ����@false = �ł��邾������
constexpr bool VSYNC_ENABLED = true;
//�����r���[���ʂ�Z�l
constexpr float SCREEN_DEPTH = 1000.0f;
//�߂��r���[���ʂ�Z�l
constexpr float SCREEN_NEAR = 0.1f;

//�O���t�B�b�N�X�N���X
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	//�������֐�
	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	//�V���b�g�_�E���֐�
	void Shutdown();
	//�t���[�����o�ŌĂ΂��֐�
	bool Frame(float rotationY);
	//�����_�����O�֐�
	bool Render();
private:
	/*
	RenderToTexture�́A�V�����v���C�x�[�g�����_�����O�@�\��1�ł��B
	���ׂẴO���t�B�b�N�������_�����O����ꏊ�Ƃ��āA�����_�����O���e�N�X�`���r���[�ɐݒ肵�܂��B
	�V�[���������_�����O���ꂽ��́AD3DClass�I�u�W�F�N�g���g�p���ă����_�����O�ʒu��ʏ�̃o�b�N�o�b�t�@�ɖ߂��܂��B
	*/
	bool RenderToTexture();
	/*
	RenderScene�֐��́A�V�����v���C�x�[�g�����_�����O�֐���1�ł��B
	�V�[���S�̂�1�̊֐��Ń����_�����O����̂ŁA�V�[���̃����_�����O���ݒ肵�Ă��炱�̊֐����Ăяo�����Ƃ��ł��܂��B
	���̃`���[�g���A���ł́A���̊֐���RenderToTexture�֐���1��Ăяo���ăV�[�����e�N�X�`���Ƀ����_�����O���A
	����Render�֐��ōēx�Ăяo���Ēʏ�ǂ���o�b�N�o�b�t�@�Ƀ����_�����O���܂��B
	*/
	bool RenderScene();
private:
	CameraClass* m_camera;
	ModelClass* m_model;
	ShaderClass* m_shader;
	TextureShaderClass* m_texture_shader;	//2D�p
	LightClass* m_light;
	TextClass* m_text;
	RenderTextureClass* m_render_texture;
	MultiTextureShaderClass* m_multi_texture_shader;//�}���`�e�N�X�`���p�V�F�[�_�[
	BumpMapShaderClass* m_bump_map_shader;	//�o���v�}�b�v�V�F�[�_�[
	MapShaderClass* m_map_shader;			//�}�b�s���O�V�F�[�_
	FrustumClass* m_frustum;				//�����_�����O�`�F�b�N

	D3DClass* m_D3D;
	DrawTexture2DClass* m_drawTexture2D;
	ModelListClass* m_modelList;
};