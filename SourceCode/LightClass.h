#pragma once

//�V�X�e���n�̃w�b�_�[�C���N���[�h
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
	D3DXVECTOR4 m_ambient_color;	//�����̐F
	D3DXVECTOR4 m_diffuse_color;	//���C�g�̐F
	D3DXVECTOR3 m_direction;		//���C�g�̕���
	D3DXVECTOR4 m_specular_color;	//���ʔ��˂̐F
	float		m_specular_power;	//���ʔ��˂̋��x
};