#include "FrustumClass.h"

//�r���[�~����\������
void FrustumClass::ConstructFrustum(float screen_depth,D3DXMATRIX projection_matrix,D3DXMATRIX view_matrix)
{
	float z_minimum, r;
	D3DXMATRIX matrix;

	//�~����̍ŏ�Z�������v�Z
	z_minimum = -projection_matrix._43 / projection_matrix._33;
	r = screen_depth / (screen_depth - z_minimum);
	projection_matrix._33 = r;
	projection_matrix._43 = -r * z_minimum;

	//�r���[�s��ƍX�V���ꂽ�ˉe�s�񂩂�~����s����쐬
	D3DXMatrixMultiply(&matrix, &view_matrix, &projection_matrix);

	//�~����̋߂����ʂ��v�Z
	m_planes[0].a = matrix._14 + matrix._13;
	m_planes[0].b = matrix._24 + matrix._23;
	m_planes[0].c = matrix._34 + matrix._33;
	m_planes[0].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&m_planes[0], &m_planes[0]);

	//�~����̉������ʂ��v�Z
	m_planes[1].a = matrix._14 - matrix._13;
	m_planes[1].b = matrix._24 - matrix._23;
	m_planes[1].c = matrix._34 - matrix._33;
	m_planes[1].d = matrix._44 - matrix._43;
	D3DXPlaneNormalize(&m_planes[1], &m_planes[1]);

	//�~����̍����ʂ��v�Z
	m_planes[2].a = matrix._14 + matrix._11;
	m_planes[2].b = matrix._24 + matrix._21;
	m_planes[2].c = matrix._34 + matrix._31;
	m_planes[2].d = matrix._44 + matrix._41;
	D3DXPlaneNormalize(&m_planes[2], &m_planes[2]);

	//�~����̉E���ʂ��v�Z
	m_planes[3].a = matrix._14 - matrix._11;
	m_planes[3].b = matrix._24 - matrix._21;
	m_planes[3].c = matrix._34 - matrix._31;
	m_planes[3].d = matrix._44 - matrix._41;
	D3DXPlaneNormalize(&m_planes[3], &m_planes[3]);

	//�~����̏�ʂ��v�Z
	m_planes[4].a = matrix._14 - matrix._12;
	m_planes[4].b = matrix._24 - matrix._22;
	m_planes[4].c = matrix._34 - matrix._32;
	m_planes[4].d = matrix._44 - matrix._42;
	D3DXPlaneNormalize(&m_planes[4], &m_planes[4]);

	//�~����̒�ʂ��v�Z
	m_planes[5].a = matrix._14 + matrix._12;
	m_planes[5].b = matrix._24 + matrix._22;
	m_planes[5].c = matrix._34 + matrix._32;
	m_planes[5].d = matrix._44 + matrix._42;
	D3DXPlaneNormalize(&m_planes[5], &m_planes[5]);

}

//�_���~����̓����ɂ��邩�ۂ���Ԃ�
bool FrustumClass::ChechPoint(float x, float y, float z)
{
	//�_���������6�ʂ��ׂĂ̓����ɂ��邩�ǂ������m�F
	for each (auto plane in m_planes)
	{
		if (D3DXPlaneDotCoord(&plane, &D3DXVECTOR3(x, y, z)) < 0.0f)
			return false;
	}

	return true;
}
//�����̂̊e���_���~����̓����ɂ��邩�ۂ���Ԃ�
bool FrustumClass::ChechCube(float center_x, float center_y, float center_z, float radius)
{
	D3DXVECTOR3 vertexs[]{ D3DXVECTOR3(center_x - radius,center_y - radius,center_z - radius),//����O
							D3DXVECTOR3(center_x + radius,center_y - radius,center_z - radius),//�E��O
							D3DXVECTOR3(center_x - radius,center_y + radius,center_z - radius),//�����O
							D3DXVECTOR3(center_x + radius,center_y + radius,center_z - radius),//�E���O
							D3DXVECTOR3(center_x - radius,center_y - radius,center_z + radius),//���㉜
							D3DXVECTOR3(center_x + radius,center_y - radius,center_z + radius),//�E�㉜
							D3DXVECTOR3(center_x - radius,center_y + radius,center_z + radius),//������
							D3DXVECTOR3(center_x + radius,center_y + radius,center_z + radius)};//�E����
	//�����̂�1�̓_���~����ɂ��邩�ǂ������m�F
	for each (auto plane in m_planes)
	{
		bool continue_flag = false;
		for each (auto vertex in vertexs)
		{
			//8�̒��_�̂ǂꂩ���~����̓����ɂ����
			//�����̂��~����̓����ɂ���Ƃ���
			if (D3DXPlaneDotCoord(&plane, &vertex) >= 0.0f)
			{
				continue_flag = true;
				break;
			}
		}
		if (continue_flag)
			continue;

		return false;
	}

	return true;
}
//�����~����̓����ɂ��邩�ۂ���Ԃ�
bool FrustumClass::ChechSphere(float center_x, float center_y, float center_z, float radius)
{
	//���̔��a���~����̓����ɂ��邩�ǂ������m�F
	for each (auto plane in m_planes)
	{
		if (D3DXPlaneDotCoord(&plane, &D3DXVECTOR3(center_x, center_y, center_z)) < -radius)
			return false;
	}
	return true;
}
//�����`�̊e���_���~����̓����ɂ��邩�ۂ���Ԃ�
bool FrustumClass::ChechRectangle(float center_x, float center_y, float center_z, float size_x, float size_y, float size_z)
{
	D3DXVECTOR3 vertexs[]{	D3DXVECTOR3(center_x - size_x,center_y - size_y,center_z - size_z),//����O
							D3DXVECTOR3(center_x + size_x,center_y - size_y,center_z - size_z),//�E��O
							D3DXVECTOR3(center_x - size_x,center_y + size_y,center_z - size_z),//�����O
							D3DXVECTOR3(center_x + size_x,center_y + size_y,center_z - size_z),//�E���O
							D3DXVECTOR3(center_x - size_x,center_y - size_y,center_z + size_z),//���㉜
							D3DXVECTOR3(center_x + size_x,center_y - size_y,center_z + size_z),//�E�㉜
							D3DXVECTOR3(center_x - size_x,center_y + size_y,center_z + size_z),//������
							D3DXVECTOR3(center_x + size_x,center_y + size_y,center_z + size_z) };//�E����
	//�����̂�1�̓_���~����ɂ��邩�ǂ������m�F
	for each (auto plane in m_planes)
	{
		bool continue_flag = false;
		for each (auto vertex in vertexs)
		{
			if (D3DXPlaneDotCoord(&plane, &vertex))
			{
				continue_flag = true;
				break;
			}
		}
		if (continue_flag)
			continue;

		return false;
	}

	return true;
}