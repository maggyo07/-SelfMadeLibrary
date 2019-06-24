//システム系のヘッダーインクルード

//Gameシステム用ヘッダー(自作)インクルード
#include "LightClass.h"

LightClass::LightClass()
{

}

LightClass::LightClass(const LightClass& other)
{

}

LightClass::~LightClass()
{

}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambient_color = D3DXVECTOR4(red, green, blue, alpha);
	return;
}
void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuse_color = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
	return;
}

void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_specular_color = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void LightClass::SetSoecularPower(float power)
{
	m_specular_power = power;
	return;
}

D3DXVECTOR4 LightClass::GetAmbientColor()
{
	return m_ambient_color;
}

D3DXVECTOR4 LightClass::GetDiffuseColor()
{
	return m_diffuse_color;
}

D3DXVECTOR3 LightClass::GetDirection()
{
	return m_direction;
}

D3DXVECTOR4 LightClass::GetSpecularColor()
{
	return m_specular_color;
}

float LightClass::GetSpecularPower()
{
	return m_specular_power;
}
