#pragma once

#include <D3DX10math.h>

/*
各フレームConstructFrustum関数は、カメラが最初にレンダリングされた後に呼び出されます。
ConstructFrustum関数は、更新された表示位置に基づいて、ビューの円錐台の6つのプレーンを計算して格納するために、
プライベートのm_planesを使用します。そこから、点、立方体、球、または長方形のいずれかが視錐台の内側にあるかどうかにかかわらず、
見るための4つのチェック関数のいずれかを呼び出すことができます。
*/
typedef class FrustumClass
{
public:
	//ビュー円錐台構成する
	void ConstructFrustum(float screen_depth, D3DXMATRIX projection_matrix, D3DXMATRIX view_matrix);

	bool ChechPoint(float, float, float);
	bool ChechCube(float, float, float, float);
	bool ChechSphere(float, float, float, float);
	bool ChechRectangle(float, float, float, float, float, float);

private:
	D3DXPLANE m_planes[6];
}Frustum;