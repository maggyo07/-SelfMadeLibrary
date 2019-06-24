#pragma once

//プリプロセスの指示　(ビルドプリセスを高速化するために行う)
#define WIN32_LEAN_AND_MEAN

//システム系のヘッダーインクルード
#include <Windows.h>
//Gameシステム用ヘッダー(自作)インクルード
#include "InputClass.h"
#include "GraphicsClass.h"
#include "SoundClass.h"
#include "MoveCameraClass.h"
#include "AudioClass.h"
#include "TimeClass.h"
#include "FPSClass.h"
#include "CPUClass.h"

//システムクラス
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	//アプリケーションのすべての設定を行う関数
	//戻り値 bool	:入力、グラフィックスオブジェクトの作成及び初期化の成功＝true　失敗＝false
	bool Initialize();
	//シャットダウン関数
	//入力、グラフィックスオブジェクトを解放
	//ハンドルをクリーンアップする
	void Shutdown();
	//アプリケーションがループして終了するまでのすべての処理を行う
	void Run();

	//Windowsシステムメッセージを送信する関数
	//必要な情報だけを知る
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	//アプリケーションのすべての処理を行う関数
	bool Frame();
	//ウィンドウを作成する関数
	void InitializeWindows(int& screenWidth, int& screenHeight);
	//画面の設定を通常に戻し、それに関連付けられたウィンドウとハンドルを解放する関数
	void ShutdownWindows();

private:
	LPCWSTR m_application_name;	//アプリケーションの名前
	HINSTANCE m_hinstance;		//インスタンスハンドル
	HWND m_hwnd;				//ウィンドウハンドル

	GraphicsClass* m_graphics;	//グラフィックスオブジェクト(アプリケーションのすべてのグラフィックのレンダリングを処理するクラス)
	MoveCameraClass* m_move_camera;	//カメラを操作する用

	InputClass* m_input;
	AudioClass* m_audio;
	TimeClass*	m_time;
	FPSClass*	m_FPS;
	CPUClass*	m_CPU;
};

//コールバック関数
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;