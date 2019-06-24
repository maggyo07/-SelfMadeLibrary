//システム系のヘッダーインクルード

//Gameシステム用ヘッダー(自作)インクルード
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

//カメラの位置、回転を使用してビューマトリックスを構築し、更新する関数
void CameraClass::Render()
{
	D3DXVECTOR3 up, position, look_at;
	float yaw, pitch, roll;
	D3DXMATRIX rotation_matrix;

	//上を向いているベクトルを設定
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//カメラの位置を設定
	position.x = m_position_x;
	position.y = m_position_y;
	position.z = m_position_z;

	//カメラがデフォルトで探している場所を設定
	look_at.x = 0.0f;
	look_at.y = 0.0f;
	look_at.z = 1.0f;

	//yaw(Y軸)、pitch(X軸)、roll(Z軸)海連をラジアンで設定
	pitch = m_rotation_x * 0.0174532925f;
	yaw   = m_rotation_y * 0.0174532925f;
	roll  = m_rotation_z * 0.0174532925f;

	//yaw、pitch、rollの値から回転行列を作成
	D3DXMatrixRotationYawPitchRoll(&rotation_matrix, yaw, pitch, roll);

	//look_atとupベクトルを回転行列で変換し、ビューが原点で正しく回転するようにする
	D3DXVec3TransformCoord(&look_at, &look_at, &rotation_matrix);
	D3DXVec3TransformCoord(&up, &up, &rotation_matrix);

	//回転したカメラの位置をビューアの位置に変換する
	look_at = position + look_at;

	//最後に、更新された3つの絵くとるからビュー行列を作成
	D3DXMatrixLookAtLH(&m_viwe_matrix, &position, &look_at, &up);
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viwe_matrix;
	return;
}

