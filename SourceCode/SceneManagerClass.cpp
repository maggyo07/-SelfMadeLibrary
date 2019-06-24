#include "SceneManagerClass.h"
#include "MultiThread.h"
#include "HitBoxManager.h"

#include "AccessController.h"

using namespace GameL;
using System::ObjManager;
using System::HitBoxManager;

//安全にメモリを解放する
#define SAFE_DELETE(p) {if(p){delete (p); (p)=nullptr;}}

//デバックモードかどうか
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

//初期化
void SceneManagerClass::Initialize()
{

}

//登録しているシーンを実行する
void SceneManagerClass::GameLoop()
{
	SceneSwap();		//シーン切り替え
	SceneAction();		//シーンアクション実行
	SceneDraw();		//シーン描画

	//FPSの表示
	if (DEBUG_MODE)
	{
		
	}
}

void SceneManagerClass::SceneAction()
{
	//現在のシーンが存在していれば
	if (m_current_scene != nullptr)
		m_current_scene->Scene();	//シーンの進行
	
	//オブジェクト登録しているオブジェクトのアクションを実行
	ObjManager().ObjsAction();
	//当たり判定実行
	HitBoxManager().AllHitCheck();
}

void SceneManagerClass::SceneDraw()
{
	//オブジェクト登録しているオブジェクトの描画を実行
	ObjManager().ObjsDraw();
	//当たり判定の描画
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

//シーン切り替え
void SceneManagerClass::SceneSwap()
{
	if (m_current_scene == m_new_scene)
	{
		return;
	}

	if (m_current_scene != nullptr)
	{
		//シーンを変更するのでm_current_sceneのシーンチェンジ関数を呼ぶ
		m_current_scene->SceneChange(m_current_scene, m_new_scene);
	}
	else if (m_new_scene != nullptr)
	{
		//シーンを変更するのでm_new_sceneのシーンチェンジ関数を呼ぶ
		m_new_scene->SceneChange(m_current_scene, m_new_scene);
	}

	if (m_current_scene != nullptr)
	{
		//シーン切り替えによるデータの破棄
		HitBoxManager().DeleteAllListData();//HitBoxの削除
		ObjManager().ListDeleteSceneObj();	//オブジェクトリスト削除

		SAFE_DELETE(m_current_scene);
	}

	if (m_new_scene == nullptr)
	{
		SAFE_DELETE(m_current_scene);
		//CMultiThread::ThreadEnd();
		return;
	}
	m_current_scene = m_new_scene;
	m_current_scene->Initialize();	//シーンの初期化
}