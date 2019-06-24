//�O���[�o���ϐ�
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBffer
{
	float3 cameraPosition;
	float  padding;
};
//�^�C�v
struct VertexInputType
{
	float4 position	: POSITION;
	float2 tex		: TEXCOORD;
	float3 normal	: NORMAL;
};

struct PixelInputType
{
	float4 position		: SV_POSITION;
	float2 tex			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 viewDirection	:TEXCOORD1;
};

//�o�[�e�b�N�X�V�F�[�_
PixelInputType VS(VertexInputType input)
{
	PixelInputType output;
	float4 world_position;	//���_�̃��[���h�ʒu

	//�K�؂ȍs��v�Z�̂��߂Ɉʒu�x�N�g����4�P�ʂɕύX����
	input.position.w = 1.0f;

	//���[���h�A�r���[�A����ѓ��e�s��ɑ΂��Ē��_�̈ʒu���v�Z
	output.position = mul (input.position, worldMatrix);
	output.position = mul (output.position, viewMatrix);
	output.position = mul (output.position, projectionMatrix);
	
	//�s�N�Z���V�F�[�_�̃e�N�X�`�����W���i�[
	output.tex = input.tex;
	
	//�@���x�N�g�������[���h�s��ɑ΂��Ă̂݌v�Z����
	output.normal = mul (input.normal, (float3x3) worldMatrix);

	//�@���x�N�g���𐳋K��
	output.normal = normalize(output.normal);

	//���[���h�̒��_�ʒu���v�Z
	world_position = mul (input.position,worldMatrix);

	//�J�����̈ʒu�Ɛ��E�̒��_�̈ʒu�Ɋ�Â��Ď������������肷��
	output.viewDirection = cameraPosition.xyz - world_position.xyz;

	//�����v�j�x�N�g�������K��
	output.viewDirection = normalize(output.viewDirection);

	return output;	
}
