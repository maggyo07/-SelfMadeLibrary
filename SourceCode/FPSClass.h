#pragma once

#pragma comment (lib,"winmm.lib")

#include <Windows.h>
#include <mmsystem.h>

//��b�Ԃɔ�������t���[�������J�E���g���A���̃J�E���g��₦���X�V����
class FPSClass
{
public:
	FPSClass();
	void Initialize();	//�������֐�
	void Delete();	//�폜�֐�

	void Frame();	//���t���[���Ă�
	int GetFps();	//FPS���擾�p
private:
	int m_fps, m_count;
	unsigned long m_start_time;
};