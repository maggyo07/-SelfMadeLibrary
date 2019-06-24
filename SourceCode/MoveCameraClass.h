#pragma once

//カメラを動かすためのクラス
class MoveCameraClass
{
public:
	MoveCameraClass();
	~MoveCameraClass();

	//フレーム速度を設定
	void SetFrameTime(const float& );
	//Y軸回転度取得用
	void GetRotation(float&);
	void TurnLeft();
	void TurnRight();
	//随時更新用
	void Frame();	
private:
	float m_frame_time;	//フレーム速度
	float m_rotation_y;	//Y軸回転度
	float m_left_turn_speed;	//左の回転スピード
	float m_right_turn_speed;	//右の回転スピード
};