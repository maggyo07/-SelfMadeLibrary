#pragma once

#include <D3DX10math.h>

/*
�e�t���[��ConstructFrustum�֐��́A�J�������ŏ��Ƀ����_�����O���ꂽ��ɌĂяo����܂��B
ConstructFrustum�֐��́A�X�V���ꂽ�\���ʒu�Ɋ�Â��āA�r���[�̉~�����6�̃v���[�����v�Z���Ċi�[���邽�߂ɁA
�v���C�x�[�g��m_planes���g�p���܂��B��������A�_�A�����́A���A�܂��͒����`�̂����ꂩ��������̓����ɂ��邩�ǂ����ɂ�����炸�A
���邽�߂�4�̃`�F�b�N�֐��̂����ꂩ���Ăяo�����Ƃ��ł��܂��B
*/
typedef class FrustumClass
{
public:
	//�r���[�~����\������
	void ConstructFrustum(float screen_depth, D3DXMATRIX projection_matrix, D3DXMATRIX view_matrix);

	bool ChechPoint(float, float, float);
	bool ChechCube(float, float, float, float);
	bool ChechSphere(float, float, float, float);
	bool ChechRectangle(float, float, float, float, float, float);

private:
	D3DXPLANE m_planes[6];
}Frustum;