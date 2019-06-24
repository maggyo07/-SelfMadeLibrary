#include "CPUClass.h"
CPUClass::CPUClass() :m_can_read_cpu(true),m_last_sample_time(GetTickCount64()),m_cpu_usage(0)
{

}
void CPUClass::Initialize()
{
	//関数の成功失敗確認用
	PDH_STATUS status;

	//CPU使用率をポーリングするためのクエリオブジェクトを作成
	status = PdhOpenQuery(NULL, 0, &m_query_hendle);
	if (status != ERROR_SUCCESS)
		m_can_read_cpu = false;

	//システム内のすべてのCOUをポーリングするようにクエリオブジェクトを設定
	status = PdhAddCounter(m_query_hendle, TEXT("\\ Processor（_Total）\\％プロセッサ時間"),0,&m_counter_handle);
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
	//カウンタの値を受け取るデータバッファ
	PDH_FMT_COUNTERVALUE value;

	if(m_can_read_cpu)
	{
		if ((m_last_sample_time + 1000) < GetTickCount())
		{
			m_last_sample_time = GetTickCount();
			//指定されたクエリ内のすべてのカウンタの現在の生データ値を収集し、各カウンタのステータスコードを更新
			PdhCollectQueryData(m_query_hendle);
			//指定したカウンタの現在の値を特定の形式で取得
			PdhGetFormattedCounterValue(m_counter_handle, PDH_FMT_LONG, NULL, &value);
			//値を取得
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