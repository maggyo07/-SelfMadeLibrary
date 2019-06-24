#pragma once

//�J�����𓮂������߂̃N���X
class MoveCameraClass
{
public:
	MoveCameraClass();
	~MoveCameraClass();

	//�t���[�����x��ݒ�
	void SetFrameTime(const float& );
	//Y����]�x�擾�p
	void GetRotation(float&);
	void TurnLeft();
	void TurnRight();
	//�����X�V�p
	void Frame();	
private:
	float m_frame_time;	//�t���[�����x
	float m_rotation_y;	//Y����]�x
	float m_left_turn_speed;	//���̉�]�X�s�[�h
	float m_right_turn_speed;	//�E�̉�]�X�s�[�h
};