#include "CPUClass.h"
CPUClass::CPUClass() :m_can_read_cpu(true),m_last_sample_time(GetTickCount64()),m_cpu_usage(0)
{

}
void CPUClass::Initialize()
{
	//�֐��̐������s�m�F�p
	PDH_STATUS status;

	//CPU�g�p�����|�[�����O���邽�߂̃N�G���I�u�W�F�N�g���쐬
	status = PdhOpenQuery(NULL, 0, &m_query_hendle);
	if (status != ERROR_SUCCESS)
		m_can_read_cpu = false;

	//�V�X�e�����̂��ׂĂ�COU���|�[�����O����悤�ɃN�G���I�u�W�F�N�g��ݒ�
	status = PdhAddCounter(m_query_hendle, TEXT("\\ Processor�i_Total�j\\���v���Z�b�T����"),0,&m_counter_handle);
	if (status != ERROR_SUCCESS)
		m_can_read_cpu = false;
}

void CPUClass::Delete()
{
	if (m_can_read_cpu)
		PdhCloseQuery(m_query_hendle);

}

void CPUClass::Frame()
{
	//�J�E���^�̒l���󂯎��f�[�^�o�b�t�@
	PDH_FMT_COUNTERVALUE value;

	if(m_can_read_cpu)
	{
		if ((m_last_sample_time + 1000) < GetTickCount())
		{
			m_last_sample_time = GetTickCount();
			//�w�肳�ꂽ�N�G�����̂��ׂẴJ�E���^�̌��݂̐��f�[�^�l�����W���A�e�J�E���^�̃X�e�[�^�X�R�[�h���X�V
			PdhCollectQueryData(m_query_hendle);
			//�w�肵���J�E���^�̌��݂̒l�����̌`���Ŏ擾
			PdhGetFormattedCounterValue(m_counter_handle, PDH_FMT_LONG, NULL, &value);
			//�l���擾
			m_cpu_usage = value.longValue;
		}
	}
}

int CPUClass::GetCpuPercentage()
{
	if (m_can_read_cpu)
		return (int)m_cpu_usage;

	return 0;
}