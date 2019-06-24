//Gameシステム用ヘッダー(自作)インクルード
#include "SystemClass.h"

#include "AccessController.h"

using namespace System;

SystemClass::SystemClass()	:m_graphics(nullptr), m_move_camera(nullptr)
							,m_input(&Input()), m_audio(&Audio()),m_time(&Time())
							,m_FPS(&FPS()),m_CPU(&CPU())
{
}

SystemClass::SystemClass(const SystemClass &other)
{

}

SystemClass::~SystemClass()
{

}

//アプリケーションのすべての設定を行う関数
//戻り値 bool	:入力、グラフィックスオブジェクトの作成及び初期化の成功＝true　失敗＝false
bool SystemClass::Initialize()
{
	bool result;	//結果情報格納用
	
	//画面の大きさ
	//変数を関数に送る前に、画面の幅と高さをゼロに初期化する
	int screenWidth = 0, screenHeight = 0;

	//Windows APIを初期化する
	InitializeWindows(screenWidth, screenHeight);

	//入力オブジェクトを初期化します
	result = m_input->Initialize(m_hinstance,m_hwnd,screenWidth,screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"入力オブジェクトｗ初期化できませんでした。", L"Error", MB_OK);
		return false;
	}

	//グラフィックスオブジェクトを作成します。このオブジェクトは、このアプリケーションのすべてのグラフィックのレンダリングを処理します。
	m_graphics = new GraphicsClass;
	//作成に失敗したらfalseを返す
	if (!m_graphics)
		return false;

	//グラフィックオブジェクトを初期化します。
	result = m_graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	//初期化に失敗したらfalseを返す
	if (!result)
		return false;

	//カメラ操作用オブジェクトを作成
	m_move_camera = new MoveCameraClass();
	if (!m_move_camera)
		return false;

	//Audioを初期化
	m_audio->Initialize();

	//FPSを初期化
	m_FPS->Initialize();

	//CPUを初期化
	m_CPU->Initialize();

	//タイマーを初期化
	m_time->Initialize();

	//すべてが成功したらtrueを返す
	return true;
}

//シャットダウン関数
//入力、グラフィックスオブジェクトを解放
//ハンドルをクリーンアップする
void SystemClass::Shutdown()
{
	//グラフィックオブジェクトを解放する
	if (m_graphics)
	{
		m_graphics->Shutdown();
		delete m_graphics;
		m_graphics = nullptr;
	}

	//カメラ操作用オブジェクト解放
	if (m_move_camera)
	{
		delete m_move_camera;
		m_move_camera = nullptr;
	}

	//入力オブジェクトを解放
	m_input->Shutdown();

	//Audioを解放
	m_audio->Delete();
	//FPSを解放
	m_FPS->Delete();
	//CPUを解放
	m_CPU->Delete();
	//タイマーを解放
	m_time->Delete();

	//ウィンドウをシャットダウンする
	ShutdownWindows();

	//シングルトン削除
	SingletonFinalizer::Finalize();
	return;
}

//アプリケーションがループして終了するまでのすべての処理を行う
void SystemClass::Run()
{
	/*メッセージ構造
	メンバー1 hwnd		:ウィンドウハンドル
	メンバー2 lParam	:lParamメッセージの値
	メンバー3 message	:メッセージID
	メンバー4 pt.x		:カーソル位置の x 座標。
	メンバー5 pt.y		:カーソル位置の y 座標。
	メンバー6 time		:メッセージがポストされた時刻。
	メンバー7 wParam	:wParamメッセージの値
	*/
	MSG msg;

	//メッセージ構造を初期化
	ZeroMemory(&msg, sizeof(MSG));
	
	//ウィンドウまたはユーザーからの終了メッセージがあるまでループする
	bool done = false;
	while (!done)
	{
		//ウィンドウメッセージを処理します
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); //仮想キーメッセージを文字メッセージへ変換
			DispatchMessage(&msg);	//取得したメッセージをディスパッチ（送出）する
		}

		//ウィンドウがアプリケーションを終了するように指示したら、終了する
		if (msg.message == WM_QUIT)
			done = true;
		else
		{
			//それ以外の場合はフレーム処理を行う
			if (!Frame())
				done = true;
		}

		//ユーザーがエスケープを押して終了したかどうかを確認
		if (m_input->GetKey(DIK_ESCAPE))
			done = true;
		if (m_input->GetJoypadButton(0))
			done = true;
	}

	return;
}

//アプリケーションのすべての処理を行う関数
bool SystemClass::Frame()
{
	//システム統計を更新
	m_time->Frame();
	m_FPS->Frame();
	m_CPU->Frame();
	//入力フレーム処理を行う
	if (!m_input->Frame())
		return false;

	//現在のカメラ回転度(Y軸)
	float rotationY;
	//更新
	m_move_camera->Frame();
	m_move_camera->GetRotation(rotationY);

	//グラフィックスオブジェクトのフレーム処理を行う
	if (!m_graphics->Frame(rotationY))
		return false;

	//最後にグラフィックをスクリーンにレンダリング
	if (!m_graphics->Render())
		return false;

	return true;
}
//Windowsシステムメッセージを送信する関数
//必要な情報だけを知る
LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd,umsg,wparam,lparam);
}

//ウィンドウを作成する関数
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	//ウィンドウクラスのパラメタ
	WNDCLASSEX wc;
	//プリンタドライバ
	DEVMODE dmScreenSettings;
	//ウィンドウの位置　
	int posx, posy;

	//このオブジェクトへの外部ポインタを取得
	ApplicationHandle = this;

	//このアプリケーションのインスタンスを取得
	m_hinstance = GetModuleHandle(NULL);

	//アプリケーションの名前を付ける
	m_application_name = L"エンジン";
	
	//デフォルトの設定でWindowsクラスを設定
	//http://www-higashi.ist.osaka-u.ac.jp/~k-maeda/vcpp/sec1-3wnddetail.html#back
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;			//ウィンドウスタイル
	wc.lpfnWndProc = WndProc;								//コールバック関数へのポインタ
	wc.cbClsExtra = 0;										//ウィンドウ倉津構造体の後ろに割り当てる補足バイト数。普通０
	wc.cbWndExtra = 0;										//ウィンドウウィスタンスの後ろに割り当てる補足バイト数。普通０
	wc.hInstance = m_hinstance;								//このクラスのためのウィンドウプロシージャがあるインスタンスハンドル
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);					//アイコンのハンドル
	wc.hIconSm = wc.hIcon;									//16x16の小さいサイズのアイコン
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);				//マウスカーソルのハンドル
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	//ウィンドウ背景色(背景描画用ブラシのハンドル)
	wc.lpszMenuName = NULL;									//デフォルトメニュー名
	wc.lpszClassName = m_application_name;					//このウィンドウクラスにつける名前
	wc.cbSize = sizeof(WNDCLASSEX);							//構造体のサイズ

	//ウィンドウクラスを登録する
	RegisterClassEx(&wc);

	//クライアントのデスクトップ画面の解像度を決定する
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//フルスクリーンで表示されているのか、ウィンドウで表示されているのかに応じて、画面の設定を行う
	if (FULL_SCREEN)
	{
		//フルスクリーンの場合、ユーザーのデスクトップと32ビットの最大サイズに画面を設定する
		//http://wisdom.sakura.ne.jp/system/winapi/win32/win154.html
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);							//この構造体のバイトサイズ
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;					//可視デバイスの表面の幅
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;				//可視デバイスの表面の高さ
		dmScreenSettings.dmBitsPerPel = 32;											//ディスプレイデバイスのカラービット数
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;	//この構造体のどのメンバを使うかを表すフラグ
		//表示設定をフルスクリーンに変更する
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		
		//ウィンドウの位置を左上隅に設定する
		posx = posy = 0;
	}
	else
	{
		//ウィンドウの場合は800x600解像度に設定する
		screenWidth = 800;
		screenHeight = 600;

		//ウィンドウを画面中央に配置する
		posx = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posy = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}
	
	//画面設定でウィンドウを作成し、ハンドルを取得する
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_application_name, m_application_name,
							WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
							posx, posy, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
	
	//ウィンドウを画面上に持ち上げ、メインフォーカスとして設定する
	ShowWindow(m_hwnd, SW_SHOW);//表示状態を設定
	SetForegroundWindow(m_hwnd);//アクティブにし、フォアグラウンドにする
	SetFocus(m_hwnd);			//キーボードフォーカスを設定

	//マウスカーソルを非表示にする
	ShowCursor(false);

	return;
}

//画面の設定を通常に戻し、それに関連付けられたウィンドウとハンドルを解放する関数
void SystemClass::ShutdownWindows()
{
	//マウスカーソルを表示する
	ShowCursor(true);

	//フルスクリーンモードを終了する場合は、表示設定を修正します
	if (FULL_SCREEN)
		ChangeDisplaySettings(NULL, 0);

	//ウィンドウを消去する
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	//アプリケーションインスタンスを消去する
	UnregisterClass(m_application_name, m_hinstance);
	m_hinstance = NULL;

	//このクラスへのポインタを解放する
	ApplicationHandle = NULL;

	return;
}
//コールバック関数
LRESULT CALLBACK WndProc(HWND hwnd,UINT umessage,WPARAM wparam,LPARAM lparam)
{
	switch (umessage)
	{
		//ウィンドウが破棄されているかどうかをチェック
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		//ウィンドウが閉じられているかどうかを確認
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		//他のすべてのメッセージは、システムクラスのメッセージハンドラに渡される
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}