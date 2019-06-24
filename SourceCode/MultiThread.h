#pragma once
#include <Windows.h>

namespace GameL
{
	typedef class CMultiThread
	{
	public:
		static void Init(void(*game_sub));
		static void Delete();
		static void ThreadEnd();			//�X���b�h���I������
		static bool GetEnd();				//�X���b�h���s�t���O��Ԃ�
	private:
		static unsigned __stdcall GameThread(void *p);	//�Q�[���X���b�h�p���\�b�h
	private:
		static bool m_execution;						//�X���b�h���s�t���O
		static CRITICAL_SECTION m_cs;					//�N���e�B�J���Z�N�V����
		static HANDLE m_thread_handoru;					//�Q�[���X���b�h�n���h��
	}GameLoop;

}