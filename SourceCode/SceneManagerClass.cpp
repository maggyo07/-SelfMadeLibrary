#include "SceneManagerClass.h"
#include "MultiThread.h"
#include "HitBoxManager.h"

#include "AccessController.h"

using namespace GameL;
using System::ObjManager;
using System::HitBoxManager;

//���S�Ƀ��������������
#define SAFE_DELETE(p) {if(p){delete (p); (p)=nullptr;}}

//�f�o�b�N���[�h���ǂ���
#ifdef _DEBUG
	#define DEBUG_MODE	(true)
#else
	#define DEBUG_MODE	(false)
#endif

SceneManagerClass::SceneManagerClass()	:m_current_scene(nullptr),m_new_scene(nullptr)
{

}
SceneManagerClass::~SceneManagerClass()
{

}

//������
void SceneManagerClass::Initialize()
{

}

//�o�^���Ă���V�[�������s����
void SceneManagerClass::GameLoop()
{
	SceneSwap();		//�V�[���؂�ւ�
	SceneAction();		//�V�[���A�N�V�������s
	SceneDraw();		//�V�[���`��

	//FPS�̕\��
	if (DEBUG_MODE)
	{
		
	}
}

void SceneManagerClass::SceneAction()
{
	//���݂̃V�[�������݂��Ă����
	if (m_current_scene != nullptr)
		m_current_scene->Scene();	//�V�[���̐i�s
	
	//�I�u�W�F�N�g�o�^���Ă���I�u�W�F�N�g�̃A�N�V���������s
	ObjManager().ObjsAction();
	//�����蔻����s
	HitBoxManager().AllHitCheck();
}

void SceneManagerClass::SceneDraw()
{
	//�I�u�W�F�N�g�o�^���Ă���I�u�W�F�N�g�̕`������s
	ObjManager().ObjsDraw();
	//�����蔻��̕`��
	if (DEBUG_MODE)
		HitBoxManager().DrawHitBox();

}

void SceneManagerClass::SetScene(CScene* scene)
{
	m_new_scene = scene;
}

CScene* SceneManagerClass::Getcene()
{
	return m_current_scene;
}

void SceneManagerClass::Delete()
{
	SAFE_DELETE(m_current_scene);
}

//�V�[���؂�ւ�
void SceneManagerClass::SceneSwap()
{
	if (m_current_scene == m_new_scene)
	{
		return;
	}

	if (m_current_scene != nullptr)
	{
		//�V�[����ύX����̂�m_current_scene�̃V�[���`�F���W�֐����Ă�
		m_current_scene->SceneChange(m_current_scene, m_new_scene);
	}
	else if (m_new_scene != nullptr)
	{
		//�V�[����ύX����̂�m_new_scene�̃V�[���`�F���W�֐����Ă�
		m_new_scene->SceneChange(m_current_scene, m_new_scene);
	}

	if (m_current_scene != nullptr)
	{
		//�V�[���؂�ւ��ɂ��f�[�^�̔j��
		HitBoxManager().DeleteAllListData();//HitBox�̍폜
		ObjManager().ListDeleteSceneObj();	//�I�u�W�F�N�g���X�g�폜

		SAFE_DELETE(m_current_scene);
	}

	if (m_new_scene == nullptr)
	{
		SAFE_DELETE(m_current_scene);
		//CMultiThread::ThreadEnd();
		return;
	}
	m_current_scene = m_new_scene;
	m_current_scene->Initialize();	//�V�[���̏�����
}