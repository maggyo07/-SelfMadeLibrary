#include "MultiThread.h"
#include <process.h>

using namespace GameL;

bool				CMultiThread::m_execution;						//�X���b�h���s�t���O
CRITICAL_SECTION	CMultiThread::m_cs;								//�N���e�B�J���Z�N�V����
HANDLE				CMultiThread::m_thread_handoru;					//�Q�[���X���b�h

void CMultiThread::Init(void(*game_sub))
{
	//�X���b�h�n���h����������
	m_thread_handoru = nullptr;
	//�X���b�h���s�t���O��ON�ɂ���
	m_execution = true;

	//�N���e�B�J���Z�N�V�����N��
	InitializeCriticalSection(&m_cs);

	//�X���b�h�쐬
	m_thread_handoru = (HANDLE)_beginthreadex(NULL, 0, GameThread, (void*)game_sub, 0, NULL);

}
void CMultiThread::Delete()
{
	//�X���b�h�I���t���O�𗧂Ă�
	ThreadEnd();
	//�X���b�h�I���m�F(�V�O�i����ԂɂȂ�܂őҋ@)
	WaitForSingleObject(m_thread_handoru, INFINITE);
	//�X���b�h�I��
	CloseHandle(m_thread_handoru);
	//�N���e�B�J���Z�N�V�����j��
	DeleteCriticalSection(&m_cs);
}
//�X���b�h���I������
void CMultiThread::ThreadEnd()
{
	//�N���e�B�J���Z�N�V���������̃X���b�h�Ŏg���Ă��Ȃ�������ׁA�g���Ă���Αҋ@
	EnterCriticalSection(&m_cs);
	//�X���b�h���s�t���O��OFF�ɂ���
	m_execution = false;
	//�N���e�B�J���Z�N�V�������
	LeaveCriticalSection(&m_cs);
}
bool CMultiThread::GetEnd()
{
	return m_execution;
}

//�Q�[���X���b�h�p���\�b�h
unsigned __stdcall CMultiThread::GameThread(void* p)
{
	void (*sub)() = (void(*)())p;

	//�Q�[�����[�v
	while (m_execution)
	{
		sub();
	}

	//�X���b�h�I��
	_endthreadex(0);
	return 0;
}