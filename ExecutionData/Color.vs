
//�O���[�o��
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//�^�C�v
struct VertexInputType
{
	float4 position : POSITION;
	float4 color	: COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color	: COLOR;
};

//�o�[�e�b�N�X�V�F�[�_�[
PixelInputType ColorVertexShader (VertexInputType input)
{
	PixelInputType output;

	//�K�؂ȍs��v�Z�̂��߂Ɉʒu�x�N�g����4�P�ʂɕύX
	input.position.w = 1.0f;

	//���[���h�A�r���[�A����ѓ��e�s��ɂ������Ē��_�̈ʒu���v�Z
	output.position = mul (input.position, worldMatrix);
	output.position = mul (output.position, viewMatrix);
	output.position = mul (output.position, projectionMatrix);

	//�g�p����s�N�Z���V�F�[�_�[�̓��͐F���i�[
	output.color = input.color;

	return output;
}

