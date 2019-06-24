//�V�X�e���n�̃w�b�_�[�C���N���[�h

//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h
#include "CameraClass.h"

CameraClass::CameraClass()
{
	m_position_x = 0.0f;
	m_position_y = 0.0f;
	m_position_z = 0.0f;

	m_rotation_x = 0.0f;
	m_rotation_y = 0.0f;
	m_rotation_z = 0.0f;
}

CameraClass::CameraClass(const CameraClass& other)
{

}

CameraClass::~CameraClass()
{

}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_position_x = x;
	m_position_y = y;
	m_position_z = z;

	return;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation_x = x;
	m_rotation_y = y;
	m_rotation_z = z;

	return;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_position_x, m_position_y, m_position_z);
}

D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotation_x, m_rotation_y, m_rotation_z);
}

//�J�����̈ʒu�A��]���g�p���ăr���[�}�g���b�N�X���\�z���A�X�V����֐�
void CameraClass::Render()
{
	D3DXVECTOR3 up, position, look_at;
	float yaw, pitch, roll;
	D3DXMATRIX rotation_matrix;

	//��������Ă���x�N�g����ݒ�
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//�J�����̈ʒu��ݒ�
	position.x = m_position_x;
	position.y = m_position_y;
	position.z = m_position_z;

	//�J�������f�t�H���g�ŒT���Ă���ꏊ��ݒ�
	look_at.x = 0.0f;
	look_at.y = 0.0f;
	look_at.z = 1.0f;

	//yaw(Y��)�Apitch(X��)�Aroll(Z��)�C�A�����W�A���Őݒ�
	pitch = m_rotation_x * 0.0174532925f;
	yaw   = m_rotation_y * 0.0174532925f;
	roll  = m_rotation_z * 0.0174532925f;

	//yaw�Apitch�Aroll�̒l�����]�s����쐬
	D3DXMatrixRotationYawPitchRoll(&rotation_matrix, yaw, pitch, roll);

	//look_at��up�x�N�g������]�s��ŕϊ����A�r���[�����_�Ő�������]����悤�ɂ���
	D3DXVec3TransformCoord(&look_at, &look_at, &rotation_matrix);
	D3DXVec3TransformCoord(&up, &up, &rotation_matrix);

	//��]�����J�����̈ʒu���r���[�A�̈ʒu�ɕϊ�����
	look_at = position + look_at;

	//�Ō�ɁA�X�V���ꂽ3�̊G���Ƃ邩��r���[�s����쐬
	D3DXMatrixLookAtLH(&m_viwe_matrix, &position, &look_at, &up);
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viwe_matrix;
	return;
}

