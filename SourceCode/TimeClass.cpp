#include "TimeClass.h"

#pragma region TimeClass
TimeClass::TimeClass()	:m_frequency(0), m_ticks_per_ms(0), m_start_time(0), m_frae_time(0)
{

}
bool TimeClass::Initialize()
{
	//�����\�^�C�}�[���T�|�[�g���Ă��邩�ǂ������m�F����
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0)
		return false;

	//���g���J�E���g��1�~���b���Ƃɉ��񍏂ނ̂����ׂ�
	m_ticks_per_ms = (float)(m_frequency/1000);

	//������\�p�t�H�[�}���X�J�E���^�̒l���擾
	QueryPerformanceCounter((LARGE_INTEGER*)&m_start_time);

	return true;
}

void TimeClass::Delete()
{

}

void TimeClass::Frame()
{
	//������\�p�t�H�[�}���X�J�E���^�̌��݂̒l
	INT64 current_time;
	//���݂̃J�E���^�l�[�ߋ��̃J�E���g�l
	float time_difference;

	//������\�p�t�H�[�}���X�J�E���^�̒l���擾
	QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
	//�������v�Z
	time_difference = (float)(current_time - m_start_time);
	//�t���[�����Ԃ��Z�o
	m_frae_time = time_difference / m_ticks_per_ms;
	//�ߋ��̃J�E���^�l���X�V
	m_start_time = current_time;

}

float TimeClass::GetTime()
{
	return m_frae_time;
}
#pragma endregion