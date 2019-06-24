//http://www.rastertek.com/tutdx11.html

//�V�X�e���n�̃w�b�_�[�C���N���[�h
#include <Windows.h>
//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h
#include "systemclass.h"
#include "AudioClass.h"
#include "Singleton.h"

#include "AccessController.h"

using namespace Graphics;

//Win32API�G���g���[�|�C���g�֐�
//����1 hInstance		:���݂̃C���X�^���X�n���h��
//����1 hPrevInstance	:�ȑO�̃C���X�^���X�n���h��(Win32�A�v���P�[�V�����ł́A���NULL)
//����1 pScmdline		:�R�}���h���C��
//����1 iCmdshow		:�\�����
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR pScmdline,int iCmdshow)
{
	ModelList().LoadModel(L"Model/DragonAnimaitionModel.fbx");
	//�V�X�e���N���X
	SystemClass& system = Singleton<SystemClass>().GetInstance();

	//�V�X�e���I�u�W�F�N�g�����������Ď��s
	if (system.Initialize())
	{
		system.Run();
	}

	//�V�X�e���I�u�W�F�N�g���V���b�g�_�E�����ĉ��
	system.Shutdown();

	if (_CrtDumpMemoryLeaks())	// ���̎��_�ŊJ������Ă��Ȃ��������̏��̕\��
	{
		MessageBox(NULL, L"���������[�N���������܂����B", L"���������[�N�G���[", MB_OK);
	}

	return 0;
}