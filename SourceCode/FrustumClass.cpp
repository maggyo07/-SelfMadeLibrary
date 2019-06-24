#include "FrustumClass.h"

//ビュー円錐台構成する
void FrustumClass::ConstructFrustum(float screen_depth,D3DXMATRIX projection_matrix,D3DXMATRIX view_matrix)
{
	float z_minimum, r;
	D3DXMATRIX matrix;

	//円錐台の最小Z距離を計算
	z_minimum = -projection_matrix._43 / projection_matrix._33;
	r = screen_depth / (screen_depth - z_minimum);
	projection_matrix._33 = r;
	projection_matrix._43 = -r * z_minimum;

	//ビュー行列と更新された射影行列から円錐台行列を作成
	D3DXMatrixMultiply(&matrix, &view_matrix, &projection_matrix);

	//円錐台の近い平面を計算
	m_planes[0].a = matrix._14 + matrix._13;
	m_planes[0].b = matrix._24 + matrix._23;
	m_planes[0].c = matrix._34 + matrix._33;
	m_planes[0].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&m_planes[0], &m_planes[0]);

	//円錐台の遠い平面を計算
	m_planes[1].a = matrix._14 - matrix._13;
	m_planes[1].b = matrix._24 - matrix._23;
	m_planes[1].c = matrix._34 - matrix._33;
	m_planes[1].d = matrix._44 - matrix._43;
	D3DXPlaneNormalize(&m_planes[1], &m_planes[1]);

	//円錐台の左側面を計算
	m_planes[2].a = matrix._14 + matrix._11;
	m_planes[2].b = matrix._24 + matrix._21;
	m_planes[2].c = matrix._34 + matrix._31;
	m_planes[2].d = matrix._44 + matrix._41;
	D3DXPlaneNormalize(&m_planes[2], &m_planes[2]);

	//円錐台の右側面を計算
	m_planes[3].a = matrix._14 - matrix._11;
	m_planes[3].b = matrix._24 - matrix._21;
	m_planes[3].c = matrix._34 - matrix._31;
	m_planes[3].d = matrix._44 - matrix._41;
	D3DXPlaneNormalize(&m_planes[3], &m_planes[3]);

	//円錐台の上面を計算
	m_planes[4].a = matrix._14 - matrix._12;
	m_planes[4].b = matrix._24 - matrix._22;
	m_planes[4].c = matrix._34 - matrix._32;
	m_planes[4].d = matrix._44 - matrix._42;
	D3DXPlaneNormalize(&m_planes[4], &m_planes[4]);

	//円錐台の底面を計算
	m_planes[5].a = matrix._14 + matrix._12;
	m_planes[5].b = matrix._24 + matrix._22;
	m_planes[5].c = matrix._34 + matrix._32;
	m_planes[5].d = matrix._44 + matrix._42;
	D3DXPlaneNormalize(&m_planes[5], &m_planes[5]);

}

//点が円錐台の内側にあるか否かを返す
bool FrustumClass::ChechPoint(float x, float y, float z)
{
	//点が視錐台の6面すべての内側にあるかどうかを確認
	for each (auto plane in m_planes)
	{
		if (D3DXPlaneDotCoord(&plane, &D3DXVECTOR3(x, y, z)) < 0.0f)
			return false;
	}

	return true;
}
//立方体の各頂点が円錐台の内側にあるか否かを返す
bool FrustumClass::ChechCube(float center_x, float center_y, float center_z, float radius)
{
	D3DXVECTOR3 vertexs[]{ D3DXVECTOR3(center_x - radius,center_y - radius,center_z - radius),//左上前
							D3DXVECTOR3(center_x + radius,center_y - radius,center_z - radius),//右上前
							D3DXVECTOR3(center_x - radius,center_y + radius,center_z - radius),//左下前
							D3DXVECTOR3(center_x + radius,center_y + radius,center_z - radius),//右下前
							D3DXVECTOR3(center_x - radius,center_y - radius,center_z + radius),//左上奥
							D3DXVECTOR3(center_x + radius,center_y - radius,center_z + radius),//右上奥
							D3DXVECTOR3(center_x - radius,center_y + radius,center_z + radius),//左下奥
							D3DXVECTOR3(center_x + radius,center_y + radius,center_z + radius)};//右下奥
	//立方体の1つの点が円錐台にあるかどうかを確認
	for each (auto plane in m_planes)
	{
		bool continue_flag = false;
		for each (auto vertex in vertexs)
		{
			//8つの頂点のどれかが円錐台の内側にあれば
			//立方体が円錐台の内側にあるとする
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
//球が円錐台の内側にあるか否かを返す
bool FrustumClass::ChechSphere(float center_x, float center_y, float center_z, float radius)
{
	//球の半径が円錐台の内側にあるかどうかを確認
	for each (auto plane in m_planes)
	{
		if (D3DXPlaneDotCoord(&plane, &D3DXVECTOR3(center_x, center_y, center_z)) < -radius)
			return false;
	}
	return true;
}
//長方形の各頂点が円錐台の内側にあるか否かを返す
bool FrustumClass::ChechRectangle(float center_x, float center_y, float center_z, float size_x, float size_y, float size_z)
{
	D3DXVECTOR3 vertexs[]{	D3DXVECTOR3(center_x - size_x,center_y - size_y,center_z - size_z),//左上前
							D3DXVECTOR3(center_x + size_x,center_y - size_y,center_z - size_z),//右上前
							D3DXVECTOR3(center_x - size_x,center_y + size_y,center_z - size_z),//左下前
							D3DXVECTOR3(center_x + size_x,center_y + size_y,center_z - size_z),//右下前
							D3DXVECTOR3(center_x - size_x,center_y - size_y,center_z + size_z),//左上奥
							D3DXVECTOR3(center_x + size_x,center_y - size_y,center_z + size_z),//右上奥
							D3DXVECTOR3(center_x - size_x,center_y + size_y,center_z + size_z),//左下奥
							D3DXVECTOR3(center_x + size_x,center_y + size_y,center_z + size_z) };//右下奥
	//立方体の1つの点が円錐台にあるかどうかを確認
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