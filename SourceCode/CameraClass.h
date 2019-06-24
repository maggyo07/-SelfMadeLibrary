#pragma once

//システム系のヘッダーインクルード
#include <D3DX10math.h>
//Gameシステム用ヘッダー(自作)インクルード

//カメラクラス
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

	//カメラの位置、回転を使用してビューマトリックスを構築し、更新する関数
	void Render();
	void GetViewMatrix(D3DXMATRIX&);

private:
	float m_position_x, m_position_y, m_position_z;
	float m_rotation_x, m_rotation_y, m_rotation_z;
	D3DXMATRIX m_viwe_matrix;
};