#include "MoveCameraClass.h"

//自作ヘッダーインクルード
#include "InputClass.h"

#include "AccessController.h"

using namespace System;

MoveCameraClass::MoveCameraClass()
{
	m_frame_time = 0.0f;	//フレーム速度
	m_rotation_y = 0.0f;	//Y軸回転度
	m_left_turn_speed = 0.0f;	//左の回転スピード
	m_right_turn_speed = 0.0f;	//右の回転スピード
}

MoveCameraClass::~MoveCameraClass()
{

}

//フレーム速度を設定
void MoveCameraClass::SetFrameTime(const float& time)
{
	m_frame_time = time;
}
//Y軸回転度取得用
void MoveCameraClass::GetRotation(float& y)
{
	y = m_rotation_y;
}

void MoveCameraClass::TurnLeft()
{
	//左キーかAキーが押されたら、カメラが左に回転する速度を上げる。
	//そうでない場合は、回転速度を遅くする
	if (Input().GetKey(DIK_LEFT) || Input().GetKey(DIK_A))
	{
		//回転速度を追加
		m_left_turn_speed += m_frame_time * 0.01f;

		//速度が最大値を超えないようにする
		if (m_left_turn_speed > (m_frame_time * 0.15f))
		{
			m_left_turn_speed = m_frame_time * 0.15f;
		}
	}
	else
	{
		//徐々に速度を落とす
		m_left_turn_speed -= m_frame_time * 0.005f;

		//速度が０未満にならないようにする
		if (m_left_turn_speed < 0.0f)
		{
			m_left_turn_speed = 0.0f;
		}
	}

	//回転速度を使用して回転を更新
	m_rotation_y -= m_left_turn_speed;
	if (m_rotation_y < 0.0f)
		m_rotation_y += 360.0f;
}

void MoveCameraClass::TurnRight()
{
	//右キーかDキーが押されたら、カメラが右に回転する速度を上げる。
	//そうでない場合は、回転速度をおそくする　
	if (Input().GetKey(DIK_RIGHT) || Input().GetKey(DIK_D))
	{
		//回転速度を追加
		m_right_turn_speed += m_frame_time * 0.01f;

		//速度が最大値を超えないようにする
		if (m_right_turn_speed > (m_frame_time * 0.15f))
		{
			m_right_turn_speed = m_frame_time * 0.15f;
		}
	}
	else
	{
		//徐々に速度を落とす
		m_right_turn_speed -= m_frame_time * 0.005f;

		//速度が０未満にならないようにする
		if (m_right_turn_speed < 0.0f)
		{
			m_right_turn_speed = 0.0f;
		}
	}

	//回転速度を使用して回転を更新
	m_rotation_y += m_right_turn_speed;
	if (m_rotation_y > 360.0f)
		m_rotation_y -= 360.0f;
}

//随時更新用
void MoveCameraClass::Frame()
{
	SetFrameTime(Time().GetTime());

	TurnLeft();
	TurnRight();
}
