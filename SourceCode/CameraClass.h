#pragma once

//�V�X�e���n�̃w�b�_�[�C���N���[�h
#include <D3DX10math.h>
//Game�V�X�e���p�w�b�_�[(����)�C���N���[�h

//�J�����N���X
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	//�J�����̈ʒu�A��]���g�p���ăr���[�}�g���b�N�X���\�z���A�X�V����֐�
	void Render();
	void GetViewMatrix(D3DXMATRIX&);

private:
	float m_position_x, m_position_y, m_position_z;
	float m_rotation_x, m_rotation_y, m_rotation_z;
	D3DXMATRIX m_viwe_matrix;
};