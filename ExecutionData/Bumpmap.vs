//�O���[�o��
cbuffer MaterixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//�^�C�v
struct VertexInputType
{
	float4 position	: POSITION;
	float2 tex		: TEXCOORD0;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
	float3 binormal	: BINORMAL;
};

struct PixelInputType
{
	float4 position	: SV_POSITION;
	float2 tex		: TEXCOORD0;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
	float3 binormal	: BINORMAL;
};

PixelInputType BumpMapVertexShader(VertexInputType input)
{
	PixelInputType output;

	//�K�؂ȍs��v�Z�̂��߂Ɉʒu�x�N�g����4�P�ʂɕύX
	input.position.w = 1.0f;

	//���[���h�A�r���[�A���e�s��ɑ΂��Ē��_�̈ʒu���v�Z
	output.position = mul (input.position, worldMatrix);
	output.position = mul (output.position,viewMatrix);
	output.position = mul (output.position,projectionMatrix);

	//�s�N�Z���V�F�[�_�̃e�N�X�`�����W���i�[
	output.tex = input.tex;

	//���[���h�s��ɑ΂��Ă̂ݖ@���x�N�g�����v�Z���Ă���A�ŏI�n�𐳋K������
	output.normal = mul(input.normal,(float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	//���[���h�s��ɑ΂��Ă̂ݐڐ��x�N�g�����v�Z���Ă���A�ŏI�l�𐳋K������
	output.tangent = mul(input.tangent,(float3x3) worldMatrix);
	output.tangent = normalize(output.tangent);

	//���[���h�s��ɑ΂��Ă̂ݓ񍀃x�N�g�����v�Z���A���ꂩ��ŏI�l�𐳋K������
	output.binormal = mul(input.binormal,(float3x3) worldMatrix);
	output.binormal = normalize(output.binormal);

	return output;
}