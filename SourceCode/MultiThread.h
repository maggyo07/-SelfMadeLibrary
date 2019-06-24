#pragma once
#include <Windows.h>

namespace GameL
{
	typedef class CMultiThread
	{
	public:
		static void Init(void(*game_sub));
		static void Delete();
		static void ThreadEnd();			//スレッドを終了する
		static bool GetEnd();				//スレッド実行フラグを返す
	private:
		static unsigned __stdcall GameThread(void *p);	//ゲームスレッド用メソッド
	private:
		static bool m_execution;						//スレッド実行フラグ
		static CRITICAL_SECTION m_cs;					//クリティカルセクション
		static HANDLE m_thread_handoru;					//ゲームスレッドハンドル
	}GameLoop;

}