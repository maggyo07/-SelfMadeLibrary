#include "MoveCameraClass.h"

//����w�b�_�[�C���N���[�h
#include "InputClass.h"

#include "AccessController.h"

using namespace System;

MoveCameraClass::MoveCameraClass()
{
	m_frame_time = 0.0f;	//�t���[�����x
	m_rotation_y = 0.0f;	//Y����]�x
	m_left_turn_speed = 0.0f;	//���̉�]�X�s�[�h
	m_right_turn_speed = 0.0f;	//�E�̉�]�X�s�[�h
}

MoveCameraClass::~MoveCameraClass()
{

}

//�t���[�����x��ݒ�
void MoveCameraClass::SetFrameTime(const float& time)
{
	m_frame_time = time;
}
//Y����]�x�擾�p
void MoveCameraClass::GetRotation(float& y)
{
	y = m_rotation_y;
}

void MoveCameraClass::TurnLeft()
{
	//���L�[��A�L�[�������ꂽ��A�J���������ɉ�]���鑬�x���グ��B
	//�����łȂ��ꍇ�́A��]���x��x������
	if (Input().GetKey(DIK_LEFT) || Input().GetKey(DIK_A))
	{
		//��]���x��ǉ�
		m_left_turn_speed += m_frame_time * 0.01f;

		//���x���ő�l�𒴂��Ȃ��悤�ɂ���
		if (m_left_turn_speed > (m_frame_time * 0.15f))
		{
			m_left_turn_speed = m_frame_time * 0.15f;
		}
	}
	else
	{
		//���X�ɑ��x�𗎂Ƃ�
		m_left_turn_speed -= m_frame_time * 0.005f;

		//���x���O�����ɂȂ�Ȃ��悤�ɂ���
		if (m_left_turn_speed < 0.0f)
		{
			m_left_turn_speed = 0.0f;
		}
	}

	//��]���x���g�p���ĉ�]���X�V
	m_rotation_y -= m_left_turn_speed;
	if (m_rotation_y < 0.0f)
		m_rotation_y += 360.0f;
}

void MoveCameraClass::TurnRight()
{
	//�E�L�[��D�L�[�������ꂽ��A�J�������E�ɉ�]���鑬�x���グ��B
	//�����łȂ��ꍇ�́A��]���x������������@
	if (Input().GetKey(DIK_RIGHT) || Input().GetKey(DIK_D))
	{
		//��]���x��ǉ�
		m_right_turn_speed += m_frame_time * 0.01f;

		//���x���ő�l�𒴂��Ȃ��悤�ɂ���
		if (m_right_turn_speed > (m_frame_time * 0.15f))
		{
			m_right_turn_speed = m_frame_time * 0.15f;
		}
	}
	else
	{
		//���X�ɑ��x�𗎂Ƃ�
		m_right_turn_speed -= m_frame_time * 0.005f;

		//���x���O�����ɂȂ�Ȃ��悤�ɂ���
		if (m_right_turn_speed < 0.0f)
		{
			m_right_turn_speed = 0.0f;
		}
	}

	//��]���x���g�p���ĉ�]���X�V
	m_rotation_y += m_right_turn_speed;
	if (m_rotation_y > 360.0f)
		m_rotation_y -= 360.0f;
}

//�����X�V�p
void MoveCameraClass::Frame()
{
	SetFrameTime(Time().GetTime());

	TurnLeft();
	TurnRight();
}
