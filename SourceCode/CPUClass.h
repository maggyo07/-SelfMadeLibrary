#pragma once

#pragma comment(lib,"pdh.lib")

#include <Pdh.h>

//毎秒発生している合計CPU使用率のパーセンテージを決定するために使用する
class CPUClass
{
public:
	CPUClass();
	void Initialize();
	void Delete();

	void Frame();
	int GetCpuPercentage();
private:
	bool m_can_read_cpu;		//システムCPU使用状態を読み取ることができるかどうかを示すフラグ
	HQUERY m_query_hendle;	//CPU使用率をポーリングするためのクエリオブジェクト
	HCOUNTER m_counter_handle;			//クエリに追加されたカウント
	unsigned long m_last_sample_time;	//毎秒1回だけサンプリングするため用
	long m_cpu_usage;		//CPU使用量
};