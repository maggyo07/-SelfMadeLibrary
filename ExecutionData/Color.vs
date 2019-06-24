
//グローバル
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//タイプ
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

//バーテックスシェーダー
PixelInputType ColorVertexShader (VertexInputType input)
{
	PixelInputType output;

	//適切な行列計算のために位置ベクトルを4単位に変更
	input.position.w = 1.0f;

	//ワールド、ビュー、および投影行列にたいして頂点の位置を計算
	output.position = mul (input.position, worldMatrix);
	output.position = mul (output.position, viewMatrix);
	output.position = mul (output.position, projectionMatrix);

	//使用するピクセルシェーダーの入力色を格納
	output.color = input.color;

	return output;
}

