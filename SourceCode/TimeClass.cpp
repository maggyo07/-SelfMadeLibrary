#include "TimeClass.h"

#pragma region TimeClass
TimeClass::TimeClass()	:m_frequency(0), m_ticks_per_ms(0), m_start_time(0), m_frae_time(0)
{

}
bool TimeClass::Initialize()
{
	//高性能タイマーをサポートしているかどうかを確認する
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0)
		return false;

	//周波数カウントが1ミリ秒ごとに何回刻むのか調べる
	m_ticks_per_ms = (float)(m_frequency/1000);

	//高分解能パフォーマンスカウンタの値を取得
	QueryPerformanceCounter((LARGE_INTEGER*)&m_start_time);

	return true;
}

void TimeClass::Delete()
{

}

void TimeClass::Frame()
{
	//高分解能パフォーマンスカウンタの現在の値
	INT64 current_time;
	//現在のカウンタ値ー過去のカウント値
	float time_difference;

	//高分解能パフォーマンスカウンタの値を取得
	QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
	//差分を計算
	time_difference = (float)(current_time - m_start_time);
	//フレーム時間を算出
	m_frae_time = time_difference / m_ticks_per_ms;
	//過去のカウンタ値を更新
	m_start_time = current_time;

}

float TimeClass::GetTime()
{
	return m_frae_time;
}
#pragma endregion