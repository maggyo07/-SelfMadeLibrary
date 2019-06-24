#pragma once

#pragma comment(lib,"pdh.lib")

#include <Pdh.h>

//���b�������Ă��鍇�vCPU�g�p���̃p�[�Z���e�[�W�����肷�邽�߂Ɏg�p����
class CPUClass
{
public:
	CPUClass();
	void Initialize();
	void Delete();

	void Frame();
	int GetCpuPercentage();
private:
	bool m_can_read_cpu;		//�V�X�e��CPU�g�p��Ԃ�ǂݎ�邱�Ƃ��ł��邩�ǂ����������t���O
	HQUERY m_query_hendle;	//CPU�g�p�����|�[�����O���邽�߂̃N�G���I�u�W�F�N�g
	HCOUNTER m_counter_handle;			//�N�G���ɒǉ����ꂽ�J�E���g
	unsigned long m_last_sample_time;	//���b1�񂾂��T���v�����O���邽�ߗp
	long m_cpu_usage;		//CPU�g�p��
};