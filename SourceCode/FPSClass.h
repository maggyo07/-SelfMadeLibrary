#pragma once

#pragma comment (lib,"winmm.lib")

#include <Windows.h>
#include <mmsystem.h>

//一秒間に発生するフレーム数をカウントし、そのカウントを絶えず更新すう
class FPSClass
{
public:
	FPSClass();
	void Initialize();	//初期化関数
	void Delete();	//削除関数

	void Frame();	//毎フレーム呼ぶ
	int GetFps();	//FPS数取得用
private:
	int m_fps, m_count;
	unsigned long m_start_time;
};