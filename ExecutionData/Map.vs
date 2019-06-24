//�O���[�o��--------------------------------
//�s��
cbuffer MaterixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//�J�����̈ʒu���
cbuffer CameraBuffer
{
	float3 camera_position;
};
//------------------------------------------

//�^�C�v
struct VertexInputType
{
	float4 position	: POSITION;				//�ʒu
	float2 tex		: TEXCOORD0;			//UV
	float3 normal	: NORMAL;				//�@��
	float3 tangent	: TANGENT;				//�ڋ�Ԃ̏��
	float3 binormal	: BINORMAL;				//normal��tangent�̊O��(Y��)
};

struct PixelInputType
{
	float4 position	: SV_POSITION;			//�ʒu
	float2 tex		: TEXCOORD0;			//UV
	float3 normal	: NORMAL;				//�@��
	float3 tangent	: TANGENT;				//�ڋ�Ԃ̏��
	float3 binormal	: BINORMAL;				//normal��tangent�̊O��(Y��)
	float3 view_direction	: TEXCOORD1;	//�J�����̕������
};

PixelInputType MapVertexShader(VertexInputType input)
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

	//���ʔ��ˌ��̌v�Z�p---------------------
	//���[���h�̒��_�̈ʒu���v�Z
	float4 world_position = mul(input.position, worldMatrix);

	//�J�����̈ʒu�ƃ��[���h�̒��_�̈ʒu�Ɋ�Â��ĕ\��������������
	output.view_direction = camera_position.xyz - world_position.xyz;

	//�\�������x�N�g���𐳋K��
	output.view_direction = normalize(output.view_direction);

	//---------------------------------------

	return output;
}