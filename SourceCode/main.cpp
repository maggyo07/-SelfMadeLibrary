//http://www.rastertek.com/tutdx11.html

//システム系のヘッダーインクルード
#include <Windows.h>
//Gameシステム用ヘッダー(自作)インクルード
#include "systemclass.h"
#include "AudioClass.h"
#include "Singleton.h"

#include "AccessController.h"

using namespace Graphics;

//Win32APIエントリーポイント関数
//引数1 hInstance		:現在のインスタンスハンドル
//引数1 hPrevInstance	:以前のインスタンスハンドル(Win32アプリケーションでは、常にNULL)
//引数1 pScmdline		:コマンドライン
//引数1 iCmdshow		:表示状態
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR pScmdline,int iCmdshow)
{
	ModelList().LoadModel(L"Model/DragonAnimaitionModel.fbx");
	//システムクラス
	SystemClass& system = Singleton<SystemClass>().GetInstance();

	//システムオブジェクトを初期化して実行
	if (system.Initialize())
	{
		system.Run();
	}

	//システムオブジェクトをシャットダウンして解放
	system.Shutdown();

	if (_CrtDumpMemoryLeaks())	// この時点で開放されていないメモリの情報の表示
	{
		MessageBox(NULL, L"メモリリークが発生しました。", L"メモリリークエラー", MB_OK);
	}

	return 0;
}