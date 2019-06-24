#include "MultiThread.h"
#include <process.h>

using namespace GameL;

bool				CMultiThread::m_execution;						//スレッド実行フラグ
CRITICAL_SECTION	CMultiThread::m_cs;								//クリティカルセクション
HANDLE				CMultiThread::m_thread_handoru;					//ゲームスレッド

void CMultiThread::Init(void(*game_sub))
{
	//スレッドハンドルを初期化
	m_thread_handoru = nullptr;
	//スレッド実行フラグをONにする
	m_execution = true;

	//クリティカルセクション起動
	InitializeCriticalSection(&m_cs);

	//スレッド作成
	m_thread_handoru = (HANDLE)_beginthreadex(NULL, 0, GameThread, (void*)game_sub, 0, NULL);

}
void CMultiThread::Delete()
{
	//スレッド終了フラグを立てる
	ThreadEnd();
	//スレッド終了確認(シグナル状態になるまで待機)
	WaitForSingleObject(m_thread_handoru, INFINITE);
	//スレッド終了
	CloseHandle(m_thread_handoru);
	//クリティカルセクション破棄
	DeleteCriticalSection(&m_cs);
}
//スレッドを終了する
void CMultiThread::ThreadEnd()
{
	//クリティカルセクションが他のスレッドで使われていないかしらべ、使われていれば待機
	EnterCriticalSection(&m_cs);
	//スレッド実行フラグをOFFにする
	m_execution = false;
	//クリティカルセクション解放
	LeaveCriticalSection(&m_cs);
}
bool CMultiThread::GetEnd()
{
	return m_execution;
}

//ゲームスレッド用メソッド
unsigned __stdcall CMultiThread::GameThread(void* p)
{
	void (*sub)() = (void(*)())p;

	//ゲームループ
	while (m_execution)
	{
		sub();
	}

	//スレッド終了
	_endthreadex(0);
	return 0;
}