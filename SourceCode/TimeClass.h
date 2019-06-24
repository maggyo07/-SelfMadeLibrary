#pragma once
#include <Windows.h>

//実行フレーム間の正確な時間を測定する高精度タイマー
//主な用途は、移動に標準の時間枠を必要とするオブジェクトを同期すること
class TimeClass
{
public:
	TimeClass();
	bool Initialize();
	void Delete();
	void Frame();

	float GetTime();
private :
	INT64 m_frequency;	 //周波数
	float m_ticks_per_ms;	//周波数カウントが1ミリ秒ごとに何回刻むのか
	INT64 m_start_time;	 //1フレーム前のカウンタ
	float m_frae_time;	 //フレーム時間
};