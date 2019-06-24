#pragma once

//システム系のヘッダーインクルード
#include <D3DX10math.h>

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();
	
	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSoecularPower(float);

	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();
	D3DXVECTOR4 GetSpecularColor();
	float GetSpecularPower();

private:
	D3DXVECTOR4 m_ambient_color;	//環境光の色
	D3DXVECTOR4 m_diffuse_color;	//ライトの色
	D3DXVECTOR3 m_direction;		//ライトの方向
	D3DXVECTOR4 m_specular_color;	//鏡面反射の色
	float		m_specular_power;	//鏡面反射の強度
};