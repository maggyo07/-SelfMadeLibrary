#pragma once
#include <Windows.h>

//���s�t���[���Ԃ̐��m�Ȏ��Ԃ𑪒肷�鍂���x�^�C�}�[
//��ȗp�r�́A�ړ��ɕW���̎��Ԙg��K�v�Ƃ���I�u�W�F�N�g�𓯊����邱��
class TimeClass
{
public:
	TimeClass();
	bool Initialize();
	void Delete();
	void Frame();

	float GetTime();
private :
	INT64 m_frequency;	 //���g��
	float m_ticks_per_ms;	//���g���J�E���g��1�~���b���Ƃɉ��񍏂ނ̂�
	INT64 m_start_time;	 //1�t���[���O�̃J�E���^
	float m_frae_time;	 //�t���[������
};