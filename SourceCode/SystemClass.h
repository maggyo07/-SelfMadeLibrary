#pragma once

//�v���v���Z�X�̎w���@(�r���h�v���Z�X�����������邽�߂ɍs��)
#define WIN32_LEAN_AND_MEAN

//�V�X�e���n�̃w�b�_�[�C���N���[�h
#include <Windows.h>
//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h
#include "InputClass.h"
#include "GraphicsClass.h"
#include "SoundClass.h"
#include "MoveCameraClass.h"
#include "AudioClass.h"
#include "TimeClass.h"
#include "FPSClass.h"
#include "CPUClass.h"

//�V�X�e���N���X
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	//�A�v���P�[�V�����̂��ׂĂ̐ݒ���s���֐�
	//�߂�l bool	:���́A�O���t�B�b�N�X�I�u�W�F�N�g�̍쐬�y�я������̐�����true�@���s��false
	bool Initialize();
	//�V���b�g�_�E���֐�
	//���́A�O���t�B�b�N�X�I�u�W�F�N�g�����
	//�n���h�����N���[���A�b�v����
	void Shutdown();
	//�A�v���P�[�V���������[�v���ďI������܂ł̂��ׂĂ̏������s��
	void Run();

	//Windows�V�X�e�����b�Z�[�W�𑗐M����֐�
	//�K�v�ȏ�񂾂���m��
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	//�A�v���P�[�V�����̂��ׂĂ̏������s���֐�
	bool Frame();
	//�E�B���h�E���쐬����֐�
	void InitializeWindows(int& screenWidth, int& screenHeight);
	//��ʂ̐ݒ��ʏ�ɖ߂��A����Ɋ֘A�t����ꂽ�E�B���h�E�ƃn���h�����������֐�
	void ShutdownWindows();

private:
	LPCWSTR m_application_name;	//�A�v���P�[�V�����̖��O
	HINSTANCE m_hinstance;		//�C���X�^���X�n���h��
	HWND m_hwnd;				//�E�B���h�E�n���h��

	GraphicsClass* m_graphics;	//�O���t�B�b�N�X�I�u�W�F�N�g(�A�v���P�[�V�����̂��ׂẴO���t�B�b�N�̃����_�����O����������N���X)
	MoveCameraClass* m_move_camera;	//�J�����𑀍삷��p

	InputClass* m_input;
	AudioClass* m_audio;
	TimeClass*	m_time;
	FPSClass*	m_FPS;
	CPUClass*	m_CPU;
};

//�R�[���o�b�N�֐�
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;